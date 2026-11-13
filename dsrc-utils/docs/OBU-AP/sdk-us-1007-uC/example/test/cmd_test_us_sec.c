#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libcli.h>
#include <craton/cli.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>

#include <tx_api.h>

#include "dot2_api.h"

#define DEFAULT_PSID 0x20
#define DEFAULT_MSG_LEN 100

static TX_THREAD sec_us_test_thread;
static uint8_t sec_us_test_thread_stack[40000];
static void test_us_sec_entry(ULONG input);
static void test_us_sec_api_entry(ULONG input);

void sec_timersub(struct timeval *a, struct timeval *b, struct timeval *res){
	if( a->tv_usec < b->tv_usec){
	  	if( a->tv_sec <= b->tv_sec){
		  	res->tv_usec = 0;
			res->tv_sec = 0;
		}
	  	// need sec shift
		res->tv_usec = 1000000 + a->tv_usec - b->tv_usec;
		res->tv_sec = a->tv_sec - 1 - b->tv_sec;
	}
	else{
	  	if( a->tv_sec < b->tv_sec){
		  	res->tv_usec = 0;
			res->tv_sec = 0;
		}
		res->tv_usec = a->tv_usec - b->tv_usec;
		res->tv_sec = a->tv_sec - b->tv_sec;
	}
}

void dump(unsigned int len, unsigned char* str){
  	unsigned int i;

	for( i = 0; i < len; i++){
	  	if( i != 0 && i % 4 == 0){
		  	printf(" ");
		}
		if( i != 0 && i % 16 == 0){
		  	printf("\n");
		}
	  	printf("%02x", str[i]);
	}
	printf("\nlength = %d\n", len);
}

void rand_msg(unsigned int len, unsigned char* str){
  	unsigned int i;

	for( i = 0; i < len; i++){
	  	str[i] = rand()%256;
	}
}

static unsigned int test_msg_len;
static unsigned char test_msg[DEFAULT_MSG_LEN*2];
static unsigned char sign_flag = 0, enc_flag = 0, verify_flag = 0;
static unsigned int psid = SEC_BSM_PSID;
static unsigned int repeat = 1;
unsigned char done_flag = 0;
int cmd_test_us_sec(cli_instance_t *cli, const char *command, char *argv[], int argc){
  	(void)cli;
	(void)command;
	ULONG trv;

	srand(time(NULL));
	if( argc < 6){
	  	printf("usage: test_us_sec <psid> <sign_flag> <encrypt_flag> <verify_flag> <msg_len> <repeat>\n");
		printf("<psid>: \tpsid to be encoded if <sign_flag> is set\n");
		printf("<sign_flag>:\t0: not sign; 1: sign\n");
		printf("<encrypt_flag>:\t0: not encrypt; 1: encrypt\n");
		printf("<verify_flag>:\t0: only decode, not verify; 1: decode and verify\n");
		printf("<msg_len>:\tlength of testing message, 1~%d\n", sizeof(test_msg));
		printf("<repeat>:\t# of testing rounds\n");
	}
	else{
	  	psid = atoi(argv[0]);
		sign_flag = atoi(argv[1]);
		enc_flag = atoi(argv[2]);
		verify_flag = atoi(argv[3]);
		test_msg_len = atoi(argv[4]);
		repeat = atoi(argv[5]);
		if( test_msg_len < 1 || test_msg_len > sizeof(test_msg)){
		  	test_msg_len = DEFAULT_MSG_LEN;
		}

		done_flag = 0;
		trv = tx_thread_create(&sec_us_test_thread,
		    			"sec_us_test_thread",
					test_us_sec_entry, 0,
					sec_us_test_thread_stack,
					sizeof(sec_us_test_thread_stack),
					40,
					40,
					TX_NO_TIME_SLICE, TX_AUTO_START);
		if (trv != TX_SUCCESS) {
		  	(void)fprintf(stderr, "tx_thread_create failed, trv=0x%lx\n", trv);
		}
		else{
			//	wait execution finished and removing testing thread
			while( done_flag == 0){
				(void)usleep(500000);
			}
		}

		trv = tx_thread_terminate(&sec_us_test_thread);
		if (trv != TX_SUCCESS) {
		  	(void)fprintf(stderr, "tx_thread_terminate failed, trv=0x%lx\n", trv);
		}
		trv = tx_thread_delete(&sec_us_test_thread);
		if (trv != TX_SUCCESS) {
		  	(void)fprintf(stderr, "tx_thread_delete failed, trv=0x%lx\n", trv);
		}
		(void)printf("Dot2 demo thread was removed.\n");
	}

	return CLI_OK;
}

void cmh_clear_cert_cache(void);
int clear_pri_cell_array(void);
int setup_sds(unsigned int* ee_cmh){
  	int ret = DOT2_OK, i;
	struct timeval a, b, diff;
	unsigned int root_cmh, pca_cmh, signer_cmh;
	unsigned char cert_str[512];
	unsigned int cert_str_len;
	char rca_cert_txt[] = {"80030081000881154954524920526f6f74204365727469666963617465000000004616e1788486000501010081808082e648a6a7c95a234bd9c1c910a3b37c88fcc9a3bb715a0a4dd456aeac870c45d1808018561646200ba5147aae5ee58884bcbf681776e56c6c03fc1c2055b4e76327227b2d99b9d8f03963b2940e2125fef4d348d0e37d9d6cab43d75734fa7a5ecf36"};
	char rca_pri_txt[] = {"b9232a59b2a45d16377e30b15c6383b59071500e8ec4d880823e3354f47215d0"};
	char pca_cert_txt[] = {"80030080048c1577f6f828480881144954524920504341204365727469666963617465000000004616e1788486000501010081808083beda0be09f2ca021abbb1f4c04e8444262a3bd68dc5656e2c4d82b84892c6abf80807b5074404290e6d3d198df897a2a139b627042d1e5099c72b8cd966a93b6f53576904664094f273fc4796812614ca3974bf157ee4db12c9965d669d24c2cf6bf"};
	char pca_pri_txt[] = {"003b4ebaa12fde54705dedb537a22f3eda853debdd554f447a57abcfe9d0e8d7"};
	unsigned char rca_pri_key[32], pca_pri_key[32];
	unsigned int tmp;

	// root ca
	cert_str_len = 0;
	for(i = 0; i*2 < strlen(rca_cert_txt); i++){
	  	sscanf(rca_cert_txt + i*2, "%02x", &tmp);
		cert_str[cert_str_len++] = tmp;
	}
	for(i = 0; i*2 < strlen(rca_pri_txt); i++){
	  	sscanf(rca_pri_txt + i*2, "%02x", &tmp);
		rca_pri_key[i] = tmp;
	}
	ret = sec_cmh_new(&root_cmh);
	if( ret != DOT2_OK){
	  	printf("new cmh fail\n");
	}
	else{
	  	ret = sec_cmh_store_cert_and_key(root_cmh, cert_str_len, cert_str, rca_pri_key);
		if( ret != DOT2_OK){
		  	printf("Load testing root cert failed\n");
		}
		else{
		  	printf("Load testing root cert & key OK\n");
		}
	}
	// pca
	cert_str_len = 0;
	for(i = 0; i*2 < strlen(pca_cert_txt); i++){
	  	sscanf(pca_cert_txt + i*2, "%02x", &tmp);
		cert_str[cert_str_len++] = tmp;
	}
	for(i = 0; i*2 < strlen(pca_pri_txt); i++){
	  	sscanf(pca_pri_txt + i*2, "%02x", &tmp);
		pca_pri_key[i] = tmp;
	}
	ret = sec_cmh_new(&pca_cmh);
	if( ret != DOT2_OK){
	  	printf("new cmh fail\n");
	}
	else{
	  	ret = sec_cmh_store_cert_and_key(pca_cmh, cert_str_len, cert_str, pca_pri_key);
		if( ret != DOT2_OK){
		  	printf("Load testing PCA cert failed\n");
		}
		else{
		  	printf("Load testing PCA cert & key OK\n");
		}
	}
	// end-entity cert
	printf("Generating end-entity cert for testing security service...\n");
	cert_str_len = sizeof(cert_str);
	gettimeofday(&a, NULL);
	ret = dot2_generate_cert(pca_cmh, END_ENTITY, psid, &signer_cmh, &cert_str_len, cert_str);
	gettimeofday(&b, NULL);
	sec_timersub(&b, &a, &diff);
	printf("Time spent for dot2_generate_cert(): %ld.%03ld ms\n", 
	    	diff.tv_sec*1000 + diff.tv_usec/1000, 
		diff.tv_usec%1000);
	if( ret == DOT2_OK){
		printf("Generated end-entity cert (cmh %d):\n", signer_cmh);
		(void)dump(cert_str_len, cert_str);
		printf("\n");
		*ee_cmh = signer_cmh;
	}
	else{
	  	printf("Return code of dot2_generate_cert(): %d, cmh %d\n", ret, signer_cmh);
	}
	return ret;
}

static void test_us_sec_entry(ULONG input){
  	(void)input;
  	int ret = DOT2_OK;
	unsigned int i;
  	unsigned char signed_msg[DEFAULT_MSG_LEN*8], extract_msg[DEFAULT_MSG_LEN*8];
	unsigned int signed_msg_len, extract_msg_len;
	unsigned int ee_cmh;
	struct timeval a, b, diff;
	dot2_encode_msg_t encode_para;
	dot2_decode_msg_t decode_para;

	srand(time(NULL));
	if( sign_flag != 0 || enc_flag != 0){
	  	// need certificate
		// wait until time after 1609.2 start time
		struct timeval sleep_s;
		(void)gettimeofday(&sleep_s, NULL);
		if( (sleep_s.tv_sec - 1072915234) < 5){
		  	(void)printf("Wait system time be after 1609.2 start time...\n");
		}
		while( (sleep_s.tv_sec - 1072915234) < 5){
			(void)usleep(1000000);
			(void)gettimeofday(&sleep_s, NULL);
		}
		cmh_clear_cert_cache();
		clear_pri_cell_array();
		ret = setup_sds(&ee_cmh);
	}
	i = 0;
	while( ret == DOT2_OK && i < repeat){
		usleep(100000);
		(void)rand_msg(test_msg_len, test_msg);
	  	printf("Test msg (round %d):\n", i+1);
		(void)dump(test_msg_len, test_msg);
		printf("\n");
		(void)memset(&encode_para, 0, sizeof(encode_para));
		(void)memset(&decode_para, 0, sizeof(decode_para));

		// encode msg
		encode_para.i_cmh = ee_cmh;
		encode_para.i_sign_flag = sign_flag;
		encode_para.i_enc_flag = enc_flag;
		encode_para.i_enc_recp_num = 1;	// used if enc_flag is set
		encode_para.i_enc_recp_cmh[0] = ee_cmh;
		encode_para.i_msg_len = test_msg_len;
		encode_para.i_msg = test_msg;
		encode_para.i_lifetime = 30000;	// 30s
		encode_para.o_msg_len = sizeof(signed_msg);
		encode_para.o_msg = signed_msg;
		gettimeofday(&a, NULL);
		ret = dot2_encode_msg(psid, &encode_para);
		gettimeofday(&b, NULL);
		sec_timersub(&b, &a, &diff);
		printf("Time spent for dot2_encode_msg(): %ld.%03ld ms\n", 
		    	diff.tv_sec*1000 + diff.tv_usec/1000, 
			diff.tv_usec%1000);
		if( ret == DOT2_OK){
		  	signed_msg_len = encode_para.o_msg_len;
		  	printf("Encoded 1609.2 data:\n");
			(void)dump(signed_msg_len, signed_msg);
			printf("\n");
		}
		else{
		  	printf("Return code of dot2_encode_msg(): %d\n", ret);
		}
		if( ret == DOT2_OK){
		  	// decode msg
			decode_para.i_verify_flag = verify_flag;
			decode_para.i_msg_len = signed_msg_len;
			decode_para.i_msg = signed_msg;
			decode_para.o_msg_len = sizeof(extract_msg);
			decode_para.o_msg = extract_msg;
			gettimeofday(&a, NULL);
			ret = dot2_decode_msg(psid, &decode_para);
			gettimeofday(&b, NULL);
			sec_timersub(&b, &a, &diff);
			printf("Time spent for dot2_decode_msg(): %ld.%03ld ms\n", 
			    	diff.tv_sec*1000 + diff.tv_usec/1000, 
				diff.tv_usec%1000);
			if( ret == DOT2_OK){
			  	// decode ok
				extract_msg_len = decode_para.o_msg_len;
				printf("Decoded 1609.2 data:\n");
				(void)dump(extract_msg_len, extract_msg);
				printf("\n");
			}
			else{
			  	printf("Return code of dot2_decode_msg(): %d\n", ret);
			}
		}
		i++;
	}
	done_flag = 1;
}

int cmd_test_us_sec_api(cli_instance_t *cli, const char *command, char *argv[], int argc){
  	(void)cli;
	(void)command;
	ULONG trv;
	int option;

	srand(time(NULL));
	if( argc < 1){
	  	printf("usage: test_us_sec_api <option>\n");
		printf("<option>:\t1: sign & verification; 2: encrypt & decrypt\n");
	}
	else{
	  	psid = DEFAULT_PSID;
		test_msg_len = DEFAULT_MSG_LEN;
		option = atoi(argv[0]);
		if( option == 1){
		  	sign_flag = 1;
			enc_flag = 0;
			verify_flag = 1;
		}
		else if( option == 2){
		  	sign_flag = 0;
			enc_flag = 1;
			verify_flag = 0;
		}
		else{
		  	sign_flag = 1;
			enc_flag = 0;
			verify_flag = 1;
		}
		repeat = 1;
		done_flag = 0;
		trv = tx_thread_create(&sec_us_test_thread,
		    			"sec_us_test_thread",
					test_us_sec_api_entry, 0,
					sec_us_test_thread_stack,
					sizeof(sec_us_test_thread_stack),
					40,
					40,
					TX_NO_TIME_SLICE, TX_AUTO_START);
		if (trv != TX_SUCCESS) {
		  	(void)fprintf(stderr, "tx_thread_create failed, trv=0x%lx\n", trv);
		}
		else{
			//	wait execution finished and removing testing thread
			while( done_flag == 0){
				(void)usleep(500000);
			}
		}

		trv = tx_thread_terminate(&sec_us_test_thread);
		if (trv != TX_SUCCESS) {
		  	(void)fprintf(stderr, "tx_thread_terminate failed, trv=0x%lx\n", trv);
		}
		trv = tx_thread_delete(&sec_us_test_thread);
		if (trv != TX_SUCCESS) {
		  	(void)fprintf(stderr, "tx_thread_delete failed, trv=0x%lx\n", trv);
		}
		(void)printf("Dot2 demo thread was removed.\n");
	}

	return CLI_OK;
}

static void test_us_sec_api_entry(ULONG input){
  	(void)input;
  	int ret = DOT2_OK;
	unsigned int i, ee_cmh, encoded_msg_len, decoded_msg_len;
	unsigned char encoded_msg[DEFAULT_MSG_LEN*8], decoded_msg[DEFAULT_MSG_LEN*6];
	struct timeval a, b, diff;

	srand(time(NULL));
	// wait until time after 1609.2 start time
	struct timeval sleep_s;
	(void)gettimeofday(&sleep_s, NULL);
	if( (sleep_s.tv_sec - 1072915234) < 5){
	  	(void)printf("Wait system time be after 1609.2 start time...\n");
	}
	while( (sleep_s.tv_sec - 1072915234) < 5){
		(void)usleep(1000000);
		(void)gettimeofday(&sleep_s, NULL);
	}
	cmh_clear_cert_cache();
	clear_pri_cell_array();
	ret = setup_sds(&ee_cmh);

	// set testing message
	(void)rand_msg(test_msg_len, test_msg);
  	printf("Test msg (round %d):\n", i+1);
	(void)dump(test_msg_len, test_msg);
	printf("\n");

	if ( ret == DOT2_OK && sign_flag == 1){
	  	sec_sign_t sign;
		(void)memset(&sign, 0, sizeof(sign));

		// demonstrate signing api
		sign.cmh = ee_cmh;
		sign.data_len = test_msg_len;
		sign.data = test_msg;
		sign.data_type = UNSECURE;
		sign.psid = psid;
		sign.set_gen_time = 1;	// including generation time in header
		sign.set_gen_loc = 0;	// not including generation location in header
		sign.exp_time = 0;	// not including expiration time in header
		sign.signer_id_type = CERTIFICATE;	// encode certificate in signer field
		sign.signer_id_cert_chain_len = 1;
		sign.max_cert_chain_len = 2;
		sign.sign_with_fast_ver = COMPRESSED;
		sign.ecpoint_format = COMPRESSED;
		sign.signed_data_len = sizeof(encoded_msg);
		sign.signed_data = encoded_msg;
		gettimeofday(&a, NULL);
		ret = sec_sign(&sign);
		gettimeofday(&b, NULL);
		sec_timersub(&b, &a, &diff);
		printf("Time spent for sec_sign(): %ld.%03ld ms\n", 
		    	diff.tv_sec*1000 + diff.tv_usec/1000, 
			diff.tv_usec%1000);
		if( ret == DOT2_OK){
		  	encoded_msg_len = sign.signed_data_len;
		  	printf("Encoded 1609.2 data:\n");
			(void)dump(encoded_msg_len, encoded_msg);
			printf("\n");
		}
		else{
		  	printf("Return code of sec_sign(): %d\n", ret);
		}
		if( ret == DOT2_OK){
		  	sec_sec_data_preprocessing_t recv;
			unsigned char tbs_data[DEFAULT_MSG_LEN*2];

		  	// demonstrate sec_sec_data_preprocessing & verification api
			(void)memset(&recv, 0, sizeof(recv));
			(void)memset(tbs_data, 0, sizeof(tbs_data));
			recv.data_len = encoded_msg_len;
			recv.data = encoded_msg;
			recv.raw_data_len = sizeof(decoded_msg);	// set to size of the buffer
			recv.raw_data = decoded_msg;
			recv.data_info.signed_data_len = sizeof(tbs_data);	// set buffer for keeping signed related info
			recv.data_info.signed_data = tbs_data;
			gettimeofday(&a, NULL);
			ret = sec_sec_data_preprocessing(&recv);
			gettimeofday(&b, NULL);
			sec_timersub(&b, &a, &diff);
			printf("Time spent for sec_sec_data_preprocessing(): %ld.%03ld ms\n", 
			    	diff.tv_sec*1000 + diff.tv_usec/1000, 
				diff.tv_usec%1000);
			if( ret == DOT2_OK){
			  	// decode ok
				decoded_msg_len = recv.raw_data_len;
				printf("Decoded 1609.2 data:\n");
				(void)dump(decoded_msg_len, decoded_msg);
				printf("\n");

				// demonstrate verification api
				// most fields had been filled after sec_sec_data_preprocessing
				recv.data_info.gen_time_in_past_check = 1;
				recv.data_info.validity_period = 5;
				recv.data_info.gen_time_in_future_check = 1;
				recv.data_info.future_data_period = 1;
				recv.data_info.expiry_check = 0;
				recv.data_info.gen_loc_in_cert_check = 0;
				recv.data_info.gen_loc_dis_check = 0;
				recv.data_info.cert_exp_check = 0;
				gettimeofday(&a, NULL);
				ret = sec_sec_data_verification(&recv.data_info);
				gettimeofday(&b, NULL);
				sec_timersub(&b, &a, &diff);
				printf("Time spent for sec_sec_data_verification(): %ld.%03ld ms\n", 
				    	diff.tv_sec*1000 + diff.tv_usec/1000, 
					diff.tv_usec%1000);
				printf("Return code of sec_sec_data_verification(): %d\n", ret);
			}
			else{
			  	printf("Return code of sec_sec_data_preprocessing(): %d\n", ret);
			}
		}
	}
	if( ret == DOT2_OK && enc_flag == 1){
	  	sec_encrypt_t enc;
		unsigned char hash_id[32], recp_id[8], fail_recp_id[8];

		// demonstrate encrypt api
		(void)memset(&enc, 0, sizeof(enc));
		enc.data_len = test_msg_len;
		enc.data = test_msg;
		enc.data_type = UNSECURE;

		// generate symmetric key for ecies
		if( sec_scmh_new(AES_128_CCM, 1, NULL, &enc.scmh) != DOT2_OK){
		  	ret = DOT2_FAILURE;
		}
		else{
			enc.recp_cert_num = 1;
		  	if( cmh_get_cert_hashid(ee_cmh, hash_id) != DOT2_OK){
			  	ret = DOT2_FAILURE;
			}
			else{
			  	(void)memcpy(recp_id, hash_id + 24, 8);
			  	enc.recp_cert_id_list = recp_id;
				enc.ecpoint_format = COMPRESSED;
				enc.enc_data_len = sizeof(encoded_msg);
				enc.enc_data = encoded_msg;
				enc.fail_cert_id_list = fail_recp_id;
				gettimeofday(&a, NULL);
				ret = sec_encrypt(&enc);
				gettimeofday(&b, NULL);
				sec_timersub(&b, &a, &diff);
				printf("Time spent for sec_encrypt(): %ld.%03ld ms\n", 
				    	diff.tv_sec*1000 + diff.tv_usec/1000, 
					diff.tv_usec%1000);
				if( ret == DOT2_OK){
				  	encoded_msg_len = enc.enc_data_len;
				  	printf("Encoded 1609.2 data:\n");
					(void)dump(encoded_msg_len, encoded_msg);
					printf("\n");
				}
				else{
				  	printf("Return code of sec_encrypt(): %d\n", ret);
				}
			}
		}
		if( ret == DOT2_OK){
		  	sec_decrypt_t dec;

		  	// demonstrate decrypt api
			(void)memset(&dec, 0, sizeof(dec));
			dec.data_len = encoded_msg_len;
			dec.data = encoded_msg;
			dec.cmh = ee_cmh;
			dec.decrypted_data_len = sizeof(decoded_msg);	// set to size of the buffer
			dec.decrypted_data = decoded_msg;
			gettimeofday(&a, NULL);
			ret = sec_decrypt(&dec);
			gettimeofday(&b, NULL);
			sec_timersub(&b, &a, &diff);
			printf("Time spent for sec_decrypt(): %ld.%03ld ms\n", 
			    	diff.tv_sec*1000 + diff.tv_usec/1000, 
				diff.tv_usec%1000);
			if( ret == DOT2_OK){
			  	// decode ok
				decoded_msg_len = dec.decrypted_data_len;
				printf("Decoded 1609.2 data (including encoding of version and message length):\n");
				(void)dump(decoded_msg_len, decoded_msg);
				printf("\n");
			}
			else{
			  	printf("Return code of sec_decrypt(): %d\n", ret);
			}
		}
	}
	done_flag = 1;
}

