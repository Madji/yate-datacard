/* 
   Copyright (C) 2009 - 2010
   
   Artem Makhutov <artem@makhutov.org>
   http://www.makhutov.org
   
   Dmitry Vagin <dmitry2004@yandex.ru>
*/

#include "datacarddevice.h"
#include <stdlib.h>
#include <stdio.h>


const char* CardDevice::at_cmd2str(at_cmd_t cmd)
{
	switch (cmd)
	{
		case CMD_AT:
			return "AT";

		case CMD_AT_A:
			return "ATA";

		case CMD_AT_CCWA:
			return "AT+CCWA";

		case CMD_AT_CFUN:
			return "AT+CFUN";

		case CMD_AT_CGMI:
			return "AT+CGMI";

		case CMD_AT_CGMM:
			return "AT+CGMM";

		case CMD_AT_CGMR:
			return "AT+CGMR";

		case CMD_AT_CGSN:
			return "AT+CGSN";

		case CMD_AT_CHUP:
			return "AT+CHUP";

		case CMD_AT_CIMI:
			return "AT+CIMI";

		case CMD_AT_CLIP:
			return "AT+CLIP";

		case CMD_AT_CLIR:
			return "AT+CLIR";

		case CMD_AT_CLVL:
			return "AT+CLVL";

		case CMD_AT_CMGD:
			return "AT+CMGD";

		case CMD_AT_CMGF:
			return "AT+CMGF";

		case CMD_AT_CMGR:
			return "AT+CMGR";

		case CMD_AT_CMGS:
			return "AT+CMGS";

		case CMD_AT_CNMI:
			return "AT+CNMI";

		case CMD_AT_CNUM:
			return "AT+CNUM";

		case CMD_AT_COPS:
			return "AT+COPS?";

		case CMD_AT_COPS_INIT:
			return "AT+COPS=";

		case CMD_AT_CPIN:
			return "AT+CPIN?";

		case CMD_AT_CPMS:
			return "AT+CPMS";

		case CMD_AT_CREG:
			return "AT+CREG?";

		case CMD_AT_CREG_INIT:
			return "AT+CREG=";

		case CMD_AT_CSCS:
			return "AT+CSCS";

		case CMD_AT_CSQ:
			return "AT+CSQ";

		case CMD_AT_CSSN:
			return "AT+CSSN";

		case CMD_AT_CUSD:
			return "AT+CUSD";

		case CMD_AT_CVOICE:
			return "AT^CVOICE";

		case CMD_AT_D:
			return "ATD";

		case CMD_AT_DDSETEX:
			return "AT^DDSETEX";

		case CMD_AT_DTMF:
			return "AT^DTMF";

		case CMD_AT_E:
			return "ATE";

		case CMD_AT_SMS_TEXT:
			return "SMS TEXT";

		case CMD_AT_U2DIAG:
			return "AT^U2DIAG";

		case CMD_AT_Z:
			return "ATZ";

		case CMD_AT_CMEE:
			return "AT+CMEE";

		case CMD_UNKNOWN:
			return "UNKNOWN";
		
		case CMD_AT_CSMP:
		    return "AT+CSMP";

		default:
			return "UNDEFINED";
	}
}

const char* CardDevice::at_res2str(at_res_t res)
{
	switch (res)
	{
		case RES_OK:
			return "OK";

		case RES_ERROR:
			return "ERROR";

		case RES_CMS_ERROR:
			return "+CMS ERROR";

		case RES_RING:
			return "RING";

		case RES_CSSI:
			return "+CSSI";

		case RES_CONN:
			return "^CONN";

		case RES_CEND:
			return "^CEND";

		case RES_ORIG:
			return "^ORIG";

		case RES_CONF:
			return "^CONF";

		case RES_SMMEMFULL:
			return "^SMMEMFULL";

		case RES_CSQ:
			return "+CSQ";

		case RES_RSSI:
			return "^RSSI";

		case RES_BOOT:
			return "^BOOT";

		case RES_CLIP:
			return "+CLIP";

		case RES_CMTI:
			return "+CMTI";

		case RES_CMGR:
			return "+CMGR";

		case RES_SMS_PROMPT:
			return "> ";

		case RES_CUSD:
			return "+CUSD";

		case RES_BUSY:
			return "BUSY";

		case RES_NO_DIALTONE:
			return "NO DIALTONE";

		case RES_NO_CARRIER:
			return "NO CARRIER";

		case RES_CPIN:
			return "+CPIN";

		case RES_CNUM:
			return "+CNUM";

		case RES_COPS:
			return "+COPS";

		case RES_SRVST:
			return "^SRVST";

		case RES_CREG:
			return "+CREG";

		case RES_MODE:
			return "^MODE";

		case RES_PARSE_ERROR:
			return "PARSE ERROR";

		case RES_UNKNOWN:
			return "UNKNOWN";

		default:
			return "UNDEFINED";
	}
}

char* CardDevice::at_parse_clip(char* str, size_t len)
{
	size_t	i;
	int	state;
	char*	clip = NULL;

	/*
	 * parse clip info in the following format:
	 * +CLIP: "123456789",128,...
	 */

	for (i = 0, state = 0; i < len && state < 3; i++)
	{
		switch (state)
		{
			case 0: /* search for start of the number (") */
				if (str[i] == '"')
				{
					state++;
				}
				break;

			case 1: /* mark the number */
				clip = &str[i];
				state++;
				/* fall through */

			case 2: /* search for the end of the number (") */
				if (str[i] == '"')
				{
					str[i] = '\0';
					state++;
				}
				break;
		}
	}

	if (state != 3)
	{
		return NULL;
	}

	return clip;
}

char* CardDevice::at_parse_cnum(char* str, size_t len)
{                                                                        
	size_t	i;
	int	state;
	char*	number = NULL;

	/*
	 * parse CNUM response in the following format:
	 * +CNUM: "<name>","<number>",<type>
	 */

	for (i = 0, state = 0; i < len && state < 5; i++)
	{
		switch (state)
		{
			case 0: /* search for start of the name (") */
				if (str[i] == '"')
				{
					state++;
				}
				break;

			case 1: /* search for the end of the name (") */
				if (str[i] == '"')
				{
					state++;
				}
				break;

			case 2: /* search for the start of the number (") */
				if (str[i] == '"')
				{
					state++;
				}
				break;

			case 3: /* mark the number */
				number = &str[i];
				state++;
				/* fall through */

			case 4: /* search for the end of the number (") */
				if (str[i] == '"')
				{
					str[i] = '\0';
					state++;
				}
				break;
		}
	}

	if (state != 5)
	{
		return NULL;
	}

	return number;
}

char* CardDevice::at_parse_cops(char* str, size_t len)
{
	size_t	i;
	int	state;
	char*	provider = NULL;

	/*
	 * parse COPS response in the following format:
	 * +COPS: <mode>[,<format>,<oper>]
	 */

	for (i = 0, state = 0; i < len && state < 3; i++)
	{
		switch (state)
		{
			case 0: /* search for start of the provider name (") */
				if (str[i] == '"')
				{
					state++;
				}
				break;

			case 1: /* mark the provider name */
				provider = &str[i];
				state++;
				/* fall through */

			case 2: /* search for the end of the provider name (") */
				if (str[i] == '"')
				{
					str[i] = '\0';
					state++;
				}
				break;
		}
	}

	if (state != 3)
	{
		return NULL;
	}

	return provider;
}

int CardDevice::at_parse_creg(char* str, size_t len, int* gsm_reg, int* gsm_reg_status, char** lac, char** ci)
{
	size_t	i;
	int	state;
	char*	p1 = NULL;
	char*	p2 = NULL;
	char*	p3 = NULL;
	char*	p4 = NULL;

	*gsm_reg = 0;
	*gsm_reg_status = -1;
	*lac = NULL;
	*ci  = NULL;

	/*
	 * parse CREG response in the following format:
	 * +CREG: [<p1>,]<p2>[,<p3>,<p4>]
	 */

	for (i = 0, state = 0; i < len && state < 8; i++)
	{
		switch (state)
		{
			case 0:
				if (str[i] == ':')
				{
					state++;
				}
				break;

			case 1:
				if (str[i] != ' ')
				{
					p1 = &str[i];
					state++;
				}
				/* fall through */

			case 2:
				if (str[i] == ',')
				{
					str[i] = '\0';
					state++;
				}
				break;

			case 3:
				p2 = &str[i];
				state++;
				/* fall through */

			case 4:
				if (str[i] == ',')
				{
					str[i] = '\0';
					state++;
				}
				break;

			case 5:
				if (str[i] != ' ')
				{
					p3 = &str[i];
					state++;
				}
				/* fall through */

			case 6:
				if (str[i] == ',')
				{
					str[i] = '\0';
					state++;
				}
				break;

			case 7:
				if (str[i] != ' ')
				{
					p4 = &str[i];
					state++;
				}
				break;
		}
	}

	if (state < 2)
	{
		return -1;
	}

	if ((p2 && !p3 && !p4) || (p2 && p3 && p4))
	{
		p1 = p2;
	}

	if (p1)
	{
		errno = 0;
		*gsm_reg_status = (int) strtol (p1, (char**) NULL, 10);
		if (*gsm_reg_status == 0 && errno == EINVAL)
		{
			*gsm_reg_status = -1;
			return -1;
		}

		if (*gsm_reg_status == 1 || *gsm_reg_status == 5)
		{
			*gsm_reg = 1;
		}
	}

	if (p2 && p3 && !p4)
	{
		*lac = p2;
		*ci  = p3;
	}
	else if (p3 && p4)
	{
		*lac = p3;
		*ci  = p4;
	}

	return 0;
}

int CardDevice::at_parse_cmti(char* str, size_t len)
{
	int index = -1;

	/*
	 * parse cmti info in the following format:
	 * +CMTI: <mem>,<index> 
	 */

	if (!sscanf (str, "+CMTI: %*[^,],%d", &index))
	{
	    Debug(DebugAll, "[%s] Error parsing CMTI event '%s'\n", c_str(), str);
		return -1;
	}

	return index;
}

int CardDevice::at_parse_cmgr(char* str, size_t len, int* stat, int* pdulen)
{

	/*
	 * parse cmgr info in the following format:
	 * +CMGR: <stat>,[<reserved>],<length>	                                        
	 * +CMGR: 0,,22
	 */

	if (!sscanf(str, "+CMGR: %d,,%d", stat, pdulen))
	{
	    Debug(DebugAll, "[%s] Error parsing CMGR event '%s'\n", c_str(), str);
	    return -1;
	}
	return 0;
}

int CardDevice::at_parse_cusd(char* str, size_t len, char** cusd, unsigned char* dcs)
{
	size_t	i;
	int	state;
	char*	p = NULL;

	*cusd = NULL;
	*dcs  = 0;

	/*
	 * parse cusd message in the following format:
	 * +CUSD: 0,"100,00 EURO, valid till 01.01.2010, you are using tariff "Mega Tariff". More informations *111#.",15
	 */

	for (i = 0, state = 0; i < len && state < 5; i++)
	{
		switch (state)
		{
			case 0:
				if (str[i] == '"')
				{
					state++;
				}
				break;

			case 1:
				*cusd = &str[i];
				state++;
				break;

			case 2:
				if (str[i] == '"')
				{
					str[i] = '\0';
					state++;
				}
				break;

			case 3:
				if (str[i] == ',')
				{
					state++;
				}
				break;

			case 4:
				p = &str[i];
				state++;
				break;
		}
	}

	if (state != 5)
	{
		return -1;
	}

	errno = 0;
	*dcs = (unsigned char) strtol (p, (char**) NULL, 10);
	if (errno == EINVAL)
	{
		return -1;
	}

	return 0;
}

int CardDevice::at_parse_cpin(char* str, size_t len)
{
	if(memmem(str, len, "READY", 5))
	{
		return 0;
	}
	if(memmem(str, len, "SIM PIN", 7))
	{
		Debug(DebugAll, "Datacard %s needs PIN code!\n", c_str());
		return 1;
	}
	if(memmem(str, len, "SIM PUK", 7))
	{
		Debug(DebugAll, "Datacard %s needs PUK code!\n", c_str());
		return 2;
	}
	if(memmem(str, len, "SIM PIN2", 7))
	{
		Debug(DebugAll, "Datacard %s needs PIN2 code!\n", c_str());
		return 3;
	}
	if(memmem(str, len, "SIM PUK2", 7))
	{
		Debug(DebugAll, "Datacard %s needs PUK2 code!\n", c_str());
		return 4;
	}
	
	Debug(DebugAll, "[%s] Error parsing +CPIN message: %s\n", c_str(), str);

	return -1;
}

int CardDevice::at_parse_csq(char* str, size_t len, int* rssi)
{
	/*
	 * parse +CSQ response in the following format:
	 * +CSQ: <RSSI>,<BER>
	 */

	*rssi = -1;

	if (!sscanf (str, "+CSQ: %2d,", rssi))
	{
		Debug(DebugAll, "[%s] Error parsing +CSQ result '%s'\n", c_str(), str);
		return -1;
	}

	return 0;
}

int CardDevice::at_parse_rssi(char* str, size_t len)
{
	int rssi = -1;

	/*
	 * parse RSSI info in the following format:
	 * ^RSSI:<rssi>
	 */

	if (!sscanf (str, "^RSSI:%d", &rssi))
	{
		Debug(DebugAll, "[%s] Error parsing RSSI event '%s'\n", c_str(), str);
		return -1;
	}

	return rssi;
}

int CardDevice::at_parse_mode(char* str, size_t len, int* mode, int* submode)
{
	/*
	 * parse RSSI info in the following format:
	 * ^MODE:<mode>,<submode>
	 */

	*mode    = -1;
	*submode = -1;

	if (!sscanf (str, "^MODE:%d,%d", mode, submode))
	{
		Debug(DebugAll, "[%s] Error parsing MODE event '%.*s'\n", c_str(), (int) len, str);
		return -1;
	}

	return 0;
}

/* vi: set ts=8 sw=4 sts=4 noet: */
