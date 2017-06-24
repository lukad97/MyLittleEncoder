#include "encryption.h"
#include "des/des.h"
#include "aes/aes_file_handler.h"

int encryptFile(char *name, uc* key1, uc* key2, uc* key3, Algorithm mode)
{
	switch (mode)
	{
		case des_ecb:
			return desEncryptFileECB(name, key1);
			break;
	    case des_cbc:
			return desEncryptFileCBC(name, key1);
			break;
		case tdes_ecb:
			return tdesEncryptFileECB(name, key1, key2, key3);
			break;
		case tdes_cbc:
			return tdesEncryptFileCBC(name, key1, key2, key3);
			break;
        case aes256_ecb:
            return aesEncryptFileECB(name, key1, 8);
            break;
        default:
            return UNKNOWN_ALG;
            break;
	}
}

int decryptFile(char *name, uc* key1, uc* key2, uc* key3, Algorithm mode)
{
	switch (mode)
	{
	case des_ecb:
		return desDecryptFileECB(name, key1);
		break;
	case des_cbc:
		return desDecryptFileCBC(name, key1);
		break;
	case tdes_ecb:
		return tdesDecryptFileECB(name, key1, key2, key3);
		break;
	case tdes_cbc:
		return tdesDecryptFileCBC(name, key1, key2, key3);
		break;
    case aes256_ecb:
        return aesDecryptFileECB(name, key1, 8);
        break;
    default:
        return UNKNOWN_ALG;
        break;
	}
}
