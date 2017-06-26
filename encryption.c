/**
 * @file
 * @author  Kosta Bizetic
 * @author  Luka Dojcilovic
 * @brief   Enkripcija/dekripcija fajlova
 * @details Ovaj fajl sadrzi implementaciju funkcija koje objedinjuju sve algoritme za enkripciju i dekripciju fajlova.
            Koristi se tako sto funkcijama prosledi enum Algorithm zeljenog algortima.
 */

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
        case aes128_ecb:
            return aesEncryptFile(name, key1, 4, ECB);
            break;
        case aes128_cbc:
            return aesEncryptFile(name, key1, 4, CBC);
            break;
        case aes192_ecb:
            return aesEncryptFile(name, key1, 6, ECB);
            break;
        case aes192_cbc:
            return aesEncryptFile(name, key1, 6, CBC);
            break;
        case aes256_ecb:
            return aesEncryptFile(name, key1, 8, ECB);
            break;
        case aes256_cbc:
            return aesEncryptFile(name, key1, 8, CBC);
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
        case aes128_ecb:
            return aesDecryptFile(name, key1, 4, ECB);
            break;
        case aes128_cbc:
            return aesDecryptFile(name, key1, 4, CBC);
            break;
        case aes192_ecb:
            return aesDecryptFile(name, key1, 6, ECB);
            break;
        case aes192_cbc:
            return aesDecryptFile(name, key1, 6, CBC);
            break;
        case aes256_ecb:
            return aesDecryptFile(name, key1, 8, ECB);
            break;
        case aes256_cbc:
            return aesDecryptFile(name, key1, 8, CBC);
            break;
        default:
            return UNKNOWN_ALG;
            break;
	}
}
