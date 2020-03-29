/*
  Copyright (c) 2009-2017 Dave Gamble and cJSON contributors

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "cJSON.h"


static char* read_file(const char *filename) {
    FILE *file = NULL;
    long length = 0;
    char *content = NULL;
    size_t read_chars = 0;

    /* open in read binary mode */
    file = fopen(filename, "rb");
    if (file == NULL)
    {
        goto cleanup;
    }

    /* get the length */
    if (fseek(file, 0, SEEK_END) != 0)
    {
        goto cleanup;
    }
    length = ftell(file);
    if (length < 0)
    {
        goto cleanup;
    }
    if (fseek(file, 0, SEEK_SET) != 0)
    {
        goto cleanup;
    }

    /* allocate content buffer */
    content = (char*)malloc((size_t)length + sizeof(""));
    if (content == NULL)
    {
        goto cleanup;
    }

    /* read the file into memory */
    read_chars = fread(content, sizeof(char), (size_t)length, file);
    if ((long)read_chars != length)
    {
        free(content);
        content = NULL;
        goto cleanup;
    }
    content[read_chars] = '\0';

cleanup:
    if (file != NULL)
    {
        fclose(file);
    }

    return content;
}

static int get_njson_in_directory(const char *dir) {
  struct dirent *de;
  DIR *dr = NULL;
  int nFiles=0;
  char *position_ptr=NULL;
  
  dr = opendir(dir);
  if (dr == NULL)  /* opendir returns NULL if couldn't open directory */
    { 
      printf("Could not open %s directory",dir ); 
      return nFiles; 
    } 
  while ((de = readdir(dr)) != NULL) 
    {
      position_ptr = strstr(de->d_name, ".json");
      if ( position_ptr != NULL )
	{
	  if ((long)strlen(de->d_name)-(position_ptr-de->d_name) == 5 )
	    {
	      nFiles=nFiles+1;
	    }
	}
    }
  closedir(dr);
  return nFiles;
}

static void read_directory(const char *dir, char **file_names) {

  struct dirent *de;
  DIR *dr = NULL;
  char *position_ptr=NULL;
  int i=0;
  
  dr = opendir(dir);
  while ((de = readdir(dr)) != NULL) 
    {
      position_ptr = strstr(de->d_name, ".json");
      if ( position_ptr != NULL )
	{
	  if ((long)strlen(de->d_name)-(position_ptr-de->d_name) == 5 )
	    {
	      file_names[i]=(char*)malloc((size_t)strlen(de->d_name) + sizeof(""));
	      strcpy(file_names[i],de->d_name);
	      i=i+1;
	    }
	}
    }
  closedir(dr);

  return;
}




int CJSON_CDECL main(int argc, char *argv[])
{
    char *file_name = NULL;
    cJSON *o_json = NULL;
    char *json_input = NULL;
    const char* bug= NULL;
    cJSON *json_glossary = NULL;
    char *output=NULL;
    char **file_list=NULL;
    int nFiles=0;
    
    /* print the version */
    printf("Version: %s\n", cJSON_Version());

    file_name = argv[1];
    printf("%s\n", file_name);
    
    json_input = read_file(file_name);
    printf("%s\n", json_input);
    

    o_json = cJSON_Parse(json_input);
    if ( o_json == NULL ) {
      bug= cJSON_GetErrorPtr();
      printf("found a bug?\n");
      printf("%s\n", bug);
    }
    else{
      free(json_input);
    }

    json_glossary = cJSON_GetObjectItem(o_json, "glossary");
    printf("Printing\n");
    output = cJSON_Print(o_json);
    printf("%s\n",output);
    
    /* Now some samplecode for building objects concisely: */
    /* create_objects(); */


    nFiles=get_njson_in_directory("json_dir");
    file_list=malloc(sizeof(char*)*(unsigned long)nFiles);
    read_directory("json_dir",file_list);
    printf("%d\n",nFiles);
    for ( unsigned int i=0; i<nFiles; i++ ) {
      printf("%s\n",file_list[i]);
    }

    return 0;
}
