/**
 * @file utils.c
 * @brief Utility function definitions
 * @details This file contains all the utility functions
 * @copyright @verbatim
Copyright © 2011 - 2025 Petros Koutoupis

All rights reserved.

This file is part of RapidDisk.

RapidDisk is free software: you can redistribute it and/or modify@n
		it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

RapidDisk is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with RapidDisk.  If not, see <http://www.gnu.org/licenses/>.

SPDX-License-Identifier: GPL-2.0-or-later
@endverbatim
* @author Petros Koutoupis \<petros\@petroskoutoupis.com\>
* @author Matteo Tenca \<matteo.tenca\@gmail.com\>
* @version 9.2.0
* @date 15 March 2025
*/
#include "utils.h"
#include "json.h"

#define PCRE2_CODE_UNIT_WIDTH 8
#include <pcre2.h>

/**
 * Helper funtion for free_linked_lists()
 * @param head linked list to free
 */
void clean_rc(RC_PROFILE *head) {

	/* if there is only one item in the list, remove it */
	if (head->next == NULL) {
		if (head != NULL) free(head);
		head = NULL;
	} else {
		/* get to the second to last node in the list */
		struct RC_PROFILE *current = head;
		while (current->next->next != NULL) {
			current = current->next;
		}

		/* now current points to the second to last item of the list, so let's remove current->next */
		if (current->next != NULL) {
			free(current->next);
			current->next = NULL;
		}
		clean_rc(head);
	}
}

/**
 * Helper funtion for free_linked_lists()
 * @param head linked list to free
 */
void clean_rd(RD_PROFILE *head) {

	/* if there is only one item in the list, remove it */
	if (head->next == NULL) {
		if (head != NULL) free(head);
		head = NULL;
	} else {
		/* get to the second to last node in the list */
		struct RD_PROFILE *current = head;
		while (current->next->next != NULL) {
			current = current->next;
		}

		/* now current points to the second to last item of the list, so let's remove current->next */
		if (current->next != NULL) {
			free(current->next);
			current->next = NULL;
		}
		clean_rd(head);
	}
}

/**
 * Helper funtion for free_linked_lists()
 * @param head linked list to free
 */
void clean_vp(VOLUME_PROFILE *head) {
	/* if there is only one item in the list, remove it */
	if (head->next == NULL) {
		if (head != NULL) free(head);
		head = NULL;
	} else {
		/* get to the second to last node in the list */
		struct VOLUME_PROFILE *current = head;
		while (current->next->next != NULL) {
			current = current->next;
		}

		/* now current points to the second to last item of the list, so let's remove current->next */
		if (current->next != NULL) {
			free(current->next);
			current->next = NULL;
		}
		clean_vp(head);
	}
}

/**
 * It frees the memory allocated to the linked lists
 *
 * @param rc_head The head of the linked list of RC_PROFILE structs.
 * @param rd_head The head of the linked list of RD_PROFILE structs.
 * @param vp_head The head of the linked list of VOLUME_PROFILE structs.
 */
void free_linked_lists(RC_PROFILE *rc_head, RD_PROFILE *rd_head, VOLUME_PROFILE *vp_head) {
	if (rc_head != NULL) {
		clean_rc(rc_head);
	}
	if (rd_head != NULL) {
		clean_rd(rd_head);
	}
	if (vp_head != NULL) {
		clean_vp(vp_head);
	}
}

void clean_ports(NVMET_PORTS *head) {
	/* if there is only one item in the list, remove it */
	if (head->next == NULL) {
		if (head != NULL) free(head);
		head = NULL;
	} else {
		/* get to the second to last node in the list */
		struct NVMET_PORTS *current = head;
		while (current->next->next != NULL) {
			current = current->next;
		}

		/* now current points to the second to last item of the list, so let's remove current->next */
		if (current->next != NULL) {
			free(current->next);
			current->next = NULL;
		}
		clean_ports(head);
	}
}

void clean_hosts(NVMET_ALLOWED_HOST *head) {
	/* if there is only one item in the list, remove it */
	if (head->next == NULL) {
		if (head != NULL) free(head);
		head = NULL;
	} else {
		/* get to the second to last node in the list */
		struct NVMET_ALLOWED_HOST *current = head;
		while (current->next->next != NULL) {
			current = current->next;
		}

		/* now current points to the second to last item of the list, so let's remove current->next */
		if (current->next != NULL) {
			free(current->next);
			current->next = NULL;
		}
		clean_hosts(head);
	}
}

void clean_nvmet(NVMET_PROFILE *head) {
	/* if there is only one item in the list, remove it */
	if (head->next == NULL) {
		if (head->allowed_hosts != NULL) {
			clean_hosts(head->allowed_hosts);
			head->allowed_hosts = NULL;
		}
		if (head->assigned_ports != NULL) {
			clean_ports(head->assigned_ports);
			head->assigned_ports = NULL;
		}
		if (head != NULL) free(head);
		head = NULL;
	} else {
		/* get to the second to last node in the list */
		struct NVMET_PROFILE *current = head;
		while (current->next->next != NULL) {
			current = current->next;
		}
		if (current->next->allowed_hosts != NULL) {
			clean_hosts(current->next->allowed_hosts);
			current->next->allowed_hosts = NULL;
		}
		if (current->next->assigned_ports != NULL) {
			clean_ports(current->next->assigned_ports);
			current->next->assigned_ports = NULL;
		}
		/* now current points to the second to last item of the list, so let's remove current->next */
		if (current->next != NULL) {
			free(current->next);
			current->next = NULL;
		}
		clean_nvmet(head);
	}
}

/**
 * This function frees the memory allocated for the linked lists of NVMET ports and NVMET profiles
 *
 * @param ports_head This is the head of the linked list of NVMET_PORTS.
 * @param nvmet_head This is the head of the linked list that contains the NVMET profile information.
 */
void free_nvmet_linked_lists(struct NVMET_PORTS *ports_head, struct NVMET_PROFILE *nvmet_head) {
	if (ports_head != NULL) {
		clean_ports(ports_head);
	}
	if (nvmet_head != NULL) {
		clean_nvmet(nvmet_head);
	}
}

/**
 * Helper function to replace matches of regular expression with replacement in the subject string.
 * @param re regular expression
 * @param replacement replacement string
 * @param subject haystack
 * @param result buffer to write the result string to
 * @param pcre2_result_len length of the buffer
 * @return 0 on success, -1 on error, upon error result will the contain the error message
 */
int preg_replace(const char *re, char *replacement, char *subject, char *result, size_t pcre2_result_len) {

	int rc, errorcode, res = SUCCESS;
	PCRE2_SIZE erroroffset;
	PCRE2_UCHAR reg_error[4096] = {0};

	PCRE2_SPTR pcre2_re = (PCRE2_SPTR) re;
	pcre2_code *compiled_re = pcre2_compile(pcre2_re, PCRE2_ZERO_TERMINATED, 0, &errorcode, &erroroffset, NULL);

	if (compiled_re) {
		PCRE2_UCHAR *pcre2_result = (PCRE2_UCHAR *) result;
		PCRE2_SPTR pcre2_subject = (PCRE2_SPTR) subject;
		size_t subject_length = strlen(subject);

		PCRE2_SPTR pcre2_replacement = (PCRE2_SPTR) replacement;
		size_t replacement_length = strlen(replacement);

		rc = pcre2_substitute(
				compiled_re,
				pcre2_subject,
				subject_length,
				0,
				PCRE2_SUBSTITUTE_GLOBAL,
				NULL,
				NULL,
				pcre2_replacement,
				replacement_length,
				pcre2_result,
				&pcre2_result_len
		);

		if (rc < SUCCESS) {
			// Syntax error in the replacement string
			pcre2_get_error_message(rc, reg_error, sizeof(reg_error));
			sprintf(result, "Error during replace: '%s'.", reg_error);
			res = INVALID_VALUE;
		}
		pcre2_code_free(compiled_re);
	} else {
		// Syntax error in the regular expression at erroroffset
		pcre2_get_error_message(errorcode, reg_error, sizeof(reg_error));
		sprintf(result, "Error compiling regexp at offset #%ld: '%s'.", erroroffset, reg_error);
		res = INVALID_VALUE;
	}

	return res;
}

/**
 * It takes a string and a message, and returns a string that contains the message prefixed with the string
 *
 * @param dest The destination string.
 * @param msg The message to be displayed.
 *
 * @return A pointer to the destination string.
 */
char *verbose_msg(char *dest, char *msg) {
	strcpy(dest, "%s: ");
	strcat(dest, msg);
	strcat(dest, "\n");
	return dest;
}

/**
 * Splits a string using the delimiter and put pieces in array.
 * @param input_string string to split
 * @param output_arr array of pointer to string parts
 * @param delim delimiter
 * @return the index of the last element in output_arr
 */
int split(char *input_string, char **output_arr, char *delim) {

	char *temp;
	int i;
	temp = strtok(input_string, delim);

	for(i = 0; temp != NULL; i++) {
		output_arr[i] = temp;
		temp = strtok(NULL, delim);
	}
	i--;
	return i;
}

/*
 * Return codes:
 *     0 - All RapidDisk modules inserted
 *     1 - All RapidDisk and dm-writecache modules inserted
 *    <0 - One or more RapidDisk modules are not inserted
 */
/**
 * Check for needed modules to be loaded.
 * @return 0 - All RapidDisk modules inserted, 1 - All RapidDisk and dm-writecache modules inserted, \<0 - One or more RapidDisk modules are not inserted
 */
int check_loaded_modules(void)
{
	int rc = INVALID_VALUE, n, i;
	struct dirent **list;

	if (access(SYS_RDSK, F_OK) == INVALID_VALUE) {
#ifndef SERVER
		fprintf(stderr, "Please ensure that the RapidDisk module is loaded and retry.\n");
#endif
		return -EPERM;
	}

	/* Check for rapiddisk */
	if ((i = scandir(SYS_MODULE, &list, NULL, NULL)) < 0) {
#ifndef SERVER
		fprintf(stderr, "%s: scandir: %s\n", __func__, strerror(errno));
#endif
		return -ENOENT;
	}

	/* Check for rapiddisk-cache */
	for (n = 0; n < i; n++) {
		if (strcmp(list[n]->d_name, "rapiddisk_cache") == SUCCESS) {
			rc = SUCCESS;
			break;
		}
	}

	if (rc != SUCCESS) {
#ifndef SERVER
		fprintf(stderr, "Please ensure that the RapidDisk-Cache module is loaded and retry.\n");
#endif
		list = clean_scandir(list, i);
		return rc;
	}

	/* Check for dm-writecach */
	for (n = 0; n < i; n++) {
		if (strcmp(list[n]->d_name, "dm_writecache") == SUCCESS) {
			rc = 1;
		}
	}

	list = clean_scandir(list, i);
	return rc;
}

/**
 * Free a scandir() result struct
 * @param scanlist the result of a scandir call to be freed
 * @param num the number of entry in the scandir result
 * @return always NULL
 */
struct dirent **clean_scandir(struct dirent **scanlist, int num) {
	if (scanlist != NULL) {
		while (num--) {
			if (scanlist[num] != NULL) {
				free(scanlist[num]);
				scanlist[num] =  NULL;
			}
		}
		free(scanlist);
	}
	return NULL;
}

/**
 * It prints a message to the screen
 *
 * @param ret_value The return value
 * @param message The message to print
 * @param json_flag TRUE if you want to print the message in JSON format, FALSE if you want to print the message in plain
 * text format.
 */
void print_message(int ret_value, char *message, bool json_flag) {
	if (json_flag == TRUE) {
		json_status_return(ret_value, message, NULL, FALSE);
	} else {
		printf("%s\n", message);
	}
}

/**
 * If the file name starts with a dot, return false. Otherwise, return true.
 *
 * @param list This is the directory entry that is being passed to the function.
 *
 * @return the number of files in the directory.
 */
int scandir_filter_no_dot(const struct dirent *list) {
	if (strncmp(list->d_name, ".", 1) == SUCCESS) {
		return FALSE;
	}
	return TRUE;
}

/**
 * The function `validate_size` uses PCRE2 library to perform regular expression matching and extract a
 * number from a given string converting in MiB from the specified size type.
 *
 * @param re A pointer to a null-terminated string representing the regular expression pattern to be matched.
 * @param subject The subject is a character array that contains the string on which the regular expression will be
 * applied.
 * @param error_message The `error_message` parameter is a character array where an error string will be stored.
 *
 * @return a long value representing the calculated size in MiB.
 */
unsigned long long int validate_size(const char *re, char *subject, char *error_message) {

	int rc, errorcode;
	unsigned long long res;
	PCRE2_SIZE erroroffset, pcre2_result_len, pcre2_human_size_len;
	PCRE2_UCHAR reg_error[NAMELEN] = {0};
	pcre2_match_data *pcre2_matchdata;
	PCRE2_UCHAR *pcre2_result, *pcre2_human_size;

	PCRE2_SPTR pcre2_re = (PCRE2_SPTR) re;
	pcre2_code *compiled_re = pcre2_compile(pcre2_re, PCRE2_ZERO_TERMINATED, 0, &errorcode, &erroroffset, NULL);

	if (compiled_re) {
		PCRE2_SPTR pcre2_subject = (PCRE2_SPTR) subject;
		size_t subject_length = strlen(subject);
		pcre2_matchdata = pcre2_match_data_create_from_pattern(compiled_re, NULL);

		rc = pcre2_match(
				compiled_re,
				pcre2_subject,
				subject_length,
				0,
				0,
				pcre2_matchdata,
				NULL
		);

		if (rc < 0) {
			// No match at all
			pcre2_get_error_message(rc, reg_error, sizeof(reg_error));
			sprintf(error_message, "Error during matching of size in string '%s': '%s'.", subject, reg_error);
			res = 0;
		} else if (rc == 2) {
			// Only number
			pcre2_substring_get_bynumber(pcre2_matchdata, 1, &pcre2_result, &pcre2_result_len);
			res = strtoull((char *) pcre2_result, (char **) NULL, 10);
			pcre2_substring_free(pcre2_result);
		} else if (rc == 3) {
			// Number and string (MB, GB, etc.)
			pcre2_substring_get_bynumber(pcre2_matchdata, 1, &pcre2_result, &pcre2_result_len);
			unsigned long long hsize = strtoull((char *) pcre2_result, (char **) NULL, 10);
			long double ldhsize = hsize;
			pcre2_substring_get_bynumber(pcre2_matchdata, 2, &pcre2_human_size, &pcre2_human_size_len);
			char *cpcre2_human_size = (char *) pcre2_human_size;
			if (strcmp("B", cpcre2_human_size) == 0) {
				res = floorl(ldhsize / 1024 / 1024);
			} else if (strcmp("KB", cpcre2_human_size) == 0) {
				res = floorl(ldhsize * 1000 / 1024 / 1024);
			} else if (strcmp("KiB", cpcre2_human_size) == 0) {
				res = floorl(ldhsize * 1024 / 1024 / 1024);
			} else if (strcmp("MB", cpcre2_human_size) == 0) {
				res = floorl(ldhsize * 1000 * 1000 / 1024 / 1024);
			} else if (strcmp("MiB", cpcre2_human_size) == 0) {
				res = hsize;
			} else if (strcmp("GB", cpcre2_human_size) == 0) {
				res = floorl(ldhsize * 1000 * 1000 * 1000 / 1024 / 1024);
			} else if (strcmp("GiB", cpcre2_human_size) == 0) {
				res = hsize * 1024;
			} else {
				res = 0;
			}
			pcre2_substring_free(pcre2_result);
			pcre2_substring_free(pcre2_human_size);
		}

		pcre2_code_free(compiled_re);
		pcre2_match_data_free(pcre2_matchdata);

	} else {
		// Syntax error in the regular expression at erroroffset
		pcre2_get_error_message(errorcode, reg_error, sizeof(reg_error));
		sprintf(error_message, "Error compiling regexp at offset #%ld: '%s'.", erroroffset, reg_error);
		res = 0;
	}

	return res;
}
