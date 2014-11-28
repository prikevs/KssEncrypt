#include "kss.h"
#include "getkey.h"
#include "crypt.h"

void Usage();

int main(int argc, char *argv[])
{
    char fileopen[NAMELEN];
    char filesave[NAMELEN];
    unsigned char *passwd;
    unsigned char key[KEYLEN];

    int opt;
    int encrypt, decrypt, save, usage;

    encrypt = decrypt = save = usage = 0;
    
    memset(fileopen, 0, sizeof(fileopen));
    memset(filesave, 0, sizeof(filesave));

    while((opt = getopt(argc, argv, "d:s:")) != -1){
        switch(opt){
        case 'd':
            strncpy(fileopen, optarg, NAMELEN);
            decrypt = 1;
            break;
        case 's':
            strncpy(filesave, optarg, NAMELEN);
            save = 1;
            break;
        case ':':
            usage = 1;
            printf("Option needs a value.\n");
            break;
        case '?':
            usage = 1;
            printf("Unkown option");
            break;
        }
    }
    /*
    printf("optind=%d, decrypt=%d\n", optind, decrypt);
    */
    if (usage == 1 || argc == 1 || (optind == 3 && decrypt == 1)){
        Usage();
        return 0;
    }

    if (decrypt == 0){
        encrypt = 1;
        strncpy(fileopen, argv[optind], NAMELEN);
    }

    if (save == 0){
        if (encrypt == 1){
            strncpy(filesave, fileopen, NAMELEN);
            strncat(filesave, ".kss", 4);
        }
        else if (decrypt == 1){
            if (strlen(fileopen) > 4 &&\
                    strncmp(".kss", fileopen + strlen(fileopen) - 4, 4) == 0)
                strncpy(filesave, fileopen, strlen(fileopen) - 4);
        }
    }


    if (encrypt == 1){
        passwd = getpass("PASSWD:");
        Getkey(passwd, strlen(passwd), key);
        if (Encrypt(fileopen, filesave, key, KEYLEN) < 0){
            printf("Errors occur while Encrypting file.\n");
            return 0;
        }     
    }
    else if (decrypt == 1){
        passwd = getpass("PASSWD:");
        Getkey(passwd, strlen(passwd), key);
        if (Decrypt(fileopen, filesave, key, KEYLEN) < 0){
            printf("Errors occur while Decrypting file.\n");
            return 0;
        }
    }
    return 0; 
}

void Usage()
{
    printf("usage:encryptkss [FILENAME] |-d FILENAME [-s FILENAME]\n\n");
    printf("optional arguments:\n");
    printf("  FILENAME\tEncrypt file\n");
    printf("  -d FILENAME\tDecrypt file\n");
    printf("  -s FILENAME\tNew file name (Default ORIGINAL.kss)\n");
    printf("\nOnline help: <https://github.com/PrKevince/EncryptKSS\n\n");
}
