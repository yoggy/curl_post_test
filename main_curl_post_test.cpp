#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <curl/curl.h>

const char *upload_filename = NULL;
const char *post_url = NULL;

void usage(int argc, char *argv[])
{
	printf("usage : %s [upload_filename] [post_url]\n", argv[0]);
	printf("\n");
	printf("    example\n");
	printf("        $ %s nike.jpg http://example.com/path/to/form\n", argv[0]);
	printf("\n");
	
	exit(1);
}

int main(int argc, char *argv[])
{
	if (argc != 3) usage(argc, argv);

	upload_filename = argv[1];
	post_url = argv[2];

	curl_global_init(CURL_GLOBAL_ALL);

	// read imagefile
	FILE *fd = NULL;
	struct stat file_info;
	char *buf = NULL;

	fd = fopen(upload_filename, "rb");
	if (!fd) {
		fprintf(stderr, "can't open file...filename=%s\n", upload_filename);
		return -1;
	}

	if(fstat(fileno(fd), &file_info) != 0) {
		fprintf(stderr, "fstat() failed...filename=%s\n", upload_filename);
		return -1;
	}
	
	buf = (char*)malloc(file_info.st_size);
	fread(buf, 1, file_info.st_size, fd);
	fclose(fd);
	fd = NULL;
	
	// setup libcurl
	CURL *curl = NULL;
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); // for debug...

	// setup multipart/form-data data
	struct curl_httppost *form_ptr = NULL;
    struct curl_httppost *last_ptr = NULL;
	curl_formadd(
		&form_ptr, &last_ptr,
		CURLFORM_COPYNAME, "name",
		CURLFORM_COPYCONTENTS, "camera0",
		CURLFORM_END);

	curl_formadd(
		&form_ptr, &last_ptr,
		CURLFORM_COPYNAME, "image",
		CURLFORM_CONTENTTYPE, "image/jpeg",
		CURLFORM_BUFFER, "uploadimage",
		CURLFORM_BUFFERPTR, buf,
		CURLFORM_BUFFERLENGTH, file_info.st_size,
		CURLFORM_END);
	
	curl_easy_setopt(curl, CURLOPT_URL, post_url);
	curl_easy_setopt(curl, CURLOPT_HTTPPOST, form_ptr);

	// 
	CURLcode res = curl_easy_perform(curl);
	
	if (res == CURLE_OK) {
		long response_code = 0;
		double speed_upload = 0.0;
		double total_time   = 0.0;

		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
		curl_easy_getinfo(curl, CURLINFO_SPEED_UPLOAD, &speed_upload);
		curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);
      	fprintf(stderr, "response_code=%d, speed=%.3f(bytes/sec), total_time=%.3f(sec)\n", 
			response_code, speed_upload, total_time);
	}
	else {
		fprintf(stderr, "curl_easy_perform() failed...%s\n", curl_easy_strerror(res));
	}
	
	curl_formfree(form_ptr);
	form_ptr = NULL;

	free(buf);
	buf = NULL;

	curl_easy_cleanup(curl);
	curl = NULL;

	curl_global_cleanup();

	return 0;
}
