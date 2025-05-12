#pragma once

#define RESULT_OK "OK"
#define RESULT_OK_LEN 3

#define RESULT_ERR "ERR"
#define RESULT_ERR_LEN 4

#define RESULT_CONTINUE "OK!CONTINUE"
#define RESULT_CONTINUE_LEN 12

#define RESULT_EOT "OK!EOT"
#define RESULT_EOT_LEN 7

#define RESULT_PROGRESS "OK!PROGRESS"

#define RESULT_UNSUPPORTED "ERR!UNSUPPORTED"
#define RESULT_UNSUPPORTED_LEN 16

#define RESULT_CANCEL "ERR!CANCEL"
#define RESULT_CANCEL_LEN 11

#define SEP "@"

#define RSLT_SUCCEEDED(s) (s[0] == 'O' && s[1] == 'K')
#define RSLT_FAILED(s) (s[0] != 'O' || s[1] != 'K')