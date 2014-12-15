/*++

File Name:      vhd-util-convert.c

Description:    This function could be used to finish converting RAW <-> Fixed VHD,
				Fixed VHD <-> Dynamic VHD.

Creator:        Alfred Song     	06/24/2010

History:      	Alfred Song			07/12/2010
				add function:			vhd_fiexed_to_dynamic
										vhd_dynamic_to_fixed  

--*/
#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <libgen.h>
#include <inttypes.h>
#include <sys/stat.h>

#include "libvhd.h"
#include "vhd-util.h"

#define DISK_TYPE_RAW                       0
#define DISK_TYPE_VHD_FIXED                 1
#define DISK_TYPE_VHD_DYNAMIC               2
#define DISK_TYPE_VHD_DIFF                  3

int
vhd_util_convert(int argc, char **argv)
{
	char *input_file_name;
	char *output_file_name;
	long int source_type;  /* -1 - invalid,
															 0 - raw
															 1 - fixed
															 2 - dynamic
															 3 - parent */
	long int target_type;  /* -1 - invalid,
															 0 - raw
															 1 - fixed
															 2 - dynamic
															 3 - parent */
	int c, err;
	vhd_flag_creat_t flags;

	input_file_name       = NULL;
	output_file_name      = NULL;
	source_type           = -1;
	target_type           = -1;
	err                   = -EINVAL;
	flags                 = 0;
	

	if (!argc || !argv)
		goto usage;

	optind = 0;
	while ((c = getopt(argc, argv, "i:o:s:t:h")) != -1) {
		switch (c) {
		case 'i':
			input_file_name = optarg;
			break;
		case 'o':
			output_file_name = optarg;
			break;
		case 's':
			source_type = strtol(optarg, NULL, 10);
			break;
		case 't':
			target_type = strtol(optarg, NULL, 10);
			break;
		case 'h':
		default:
			goto usage;
		}
	}

	VHD_GLOBLE_LOG("cmd parameters: "
					"input_file_name=%s, output_file_name=%s, "
					"source_type=%ld, target_type=%ld\n",
					input_file_name, output_file_name, source_type, target_type);

	if (!input_file_name || !output_file_name \
		|| source_type > 3 || source_type < 0 \
		|| target_type > 3 || target_type < 0 || optind != argc)
		goto usage;

	if (source_type == DISK_TYPE_RAW && target_type == DISK_TYPE_VHD_FIXED) {
		err = vhd_raw_to_fixed(input_file_name, output_file_name);
		if(err) {
			printf("Fail to convert RAW disk to VHD fixed disk.\n");
			VHD_GLOBLE_LOG("Fail to convert RAW disk to VHD fixed disk.\n");
			exit(1);
		}
	} else if (source_type == DISK_TYPE_VHD_FIXED && target_type == DISK_TYPE_RAW) {
		err = vhd_fixed_to_raw(input_file_name, output_file_name);
		if(err) {
			printf("Fail to convert VHD fixed disk to RAW disk.\n");
			VHD_GLOBLE_LOG("Fail to convert VHD fixed disk to RAW disk.\n");
			exit(1);
		}
	} else if (source_type == DISK_TYPE_VHD_FIXED && target_type == DISK_TYPE_VHD_DYNAMIC) {
		err = vhd_fixed_to_dynamic(input_file_name, output_file_name);
		if(err) {
			printf("Fail to convert VHD fixed disk to VHD dynamic disk. %d\n", err);
			VHD_GLOBLE_LOG("Fail to convert VHD fixed disk to VHD dynamic disk. %d\n", err);
			exit(1);
		}
	} else if (source_type == DISK_TYPE_VHD_DYNAMIC && target_type == DISK_TYPE_VHD_FIXED) {
		err = vhd_dynamic_to_fixed(input_file_name, output_file_name);
		if(err) {
			printf("Fail to convert VHD dynamic disk to VHD fixed disk. %d\n", err);
			VHD_GLOBLE_LOG("Fail to convert VHD dynamic disk to VHD fixed disk. %d\n", err);
			exit(1);
		}
	} else {
		printf("Currently, only RAW <-> FIXED, FIXED <-> DYNAMIC convert is supported.\n");
	}

	return 0; //TODO

usage:
	printf("\
options: <-i input file name> \
<-o output file name> \
<-s source type(0 - raw, 1 - fixed, 2 - dynamic, 3 - differencing)> \
<-t target type(0 - raw, 1 - fixed, 2 - dynamic, 3 - differencing)> \
[-h help]\n");
	return -EINVAL;
}

