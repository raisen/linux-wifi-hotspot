//
// Created by lakinduakash on 16/04/19.
//
/*
Copyright (c) 2019, lakinduaksh
        All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
        this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
        IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
        FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
        CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <ctype.h>
#include "util.h"
#include <regex.h>

regex_t regex;
int reti;
char msgbuf[100];

int find_str(char *find, const char **array, int length) {
    int i;

    for ( i = 0; i < length; i++ ) {
        if (strcmp(array[i], find) == 0) {
            return i;
        }
    }


    return -1;

}


void rand_str(char *dest, size_t length) {
    char charset[] = "0123456789"
                     "abcdefghijklmnopqrstuvwxyz"
                     "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    while (length-- > 0) {
        size_t index = (size_t) ((double) rand() / RAND_MAX * (sizeof charset - 1));
        *dest++ = charset[index];
    }
    *dest = '\0';
}


int isValidMacAddress(const char* mac) {
    int i = 0;
    int s = 0;

    while (*mac) {
        if (isxdigit(*mac)) {
            i++;
        }
        else if (*mac == ':' || *mac == '-') {

            if (i == 0 || i / 2 - 1 != s)
                break;

            ++s;
        }
        else {
            s = -1;
        }


        ++mac;
    }

    return (i == 12 && (s == 5 || s == 0));
}


int isValidAcceptedMacs(const char *macs){

    /* Compile regular expression */
    reti = regcomp(&regex, "^(((([0-9A-Fa-f]{2}):){5}[0-9A-Fa-f]{2}\\s*)(^((([0-9A-Fa-f]{2}):){5}[0-9A-Fa-f]{2}\\s*))*)$", REG_EXTENDED);
    if (reti) {
        //printf( "Could not compile regex\n");
        return -1;
    }

    /* Execute regular expression */
    reti = regexec(&regex, macs, 0, NULL, 0);
    if (!reti) {
        return 0;
    }
    else if (reti == REG_NOMATCH) {
        //puts("Invalid mac addresses");
        return -1;
    }
    else {
        regerror(reti, &regex, msgbuf, sizeof(msgbuf));
        //printf("Regex match failed: %s\n", msgbuf);
        return -1;
    }

}

int isValidIPaddress(const char * ip){


     /* Compile regular expression */
    reti = regcomp(&regex, 
        "^(([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])\\.){3}([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])$",
        REG_EXTENDED);
    if (reti) {
        //printf( "Could not compile regex\n");
        return -1;
    }

    /* Execute regular expression */
    reti = regexec(&regex, ip, 0, NULL, 0);
    if (!reti) {
        return 0;
    }
    else if (reti == REG_NOMATCH) {
        //puts("Invalid ip addresses");
        return -1;
    }
    else {
        regerror(reti, &regex, msgbuf, sizeof(msgbuf));
        //printf("Regex match failed: %s\n", msgbuf);
        return -1;
    }

}

int check_and_enable_firewall_dhcp(void) {
    FILE *fp;
    char buffer[128];
    int dhcp_enabled = 0;
    
    // Check if firewalld is running
    fp = popen("systemctl is-active firewalld 2>/dev/null", "r");
    if (fp == NULL) {
        return 0; // Firewalld not available, no action needed
    }
    
    if (fgets(buffer, sizeof(buffer), fp) != NULL) {
        if (strncmp(buffer, "active", 6) != 0) {
            pclose(fp);
            return 0; // Firewalld not running, no action needed
        }
    }
    pclose(fp);
    
    // Check if DHCP service is already enabled
    fp = popen("firewall-cmd --list-services 2>/dev/null | grep -w dhcp", "r");
    if (fp == NULL) {
        return -1;
    }
    
    if (fgets(buffer, sizeof(buffer), fp) != NULL) {
        dhcp_enabled = 1;
    }
    pclose(fp);
    
    if (dhcp_enabled) {
        return 0; // DHCP already enabled
    }
    
    // DHCP not enabled, enable it
    system("pkexec firewall-cmd --add-service=dhcp --permanent >/dev/null 2>&1");
    system("pkexec firewall-cmd --add-service=dhcp >/dev/null 2>&1");
    
    return 1; // DHCP was enabled by us
}

void disable_firewall_dhcp(void) {
    FILE *fp;
    char buffer[128];
    
    // Check if firewalld is running
    fp = popen("systemctl is-active firewalld 2>/dev/null", "r");
    if (fp == NULL) {
        return;
    }
    
    if (fgets(buffer, sizeof(buffer), fp) != NULL) {
        if (strncmp(buffer, "active", 6) != 0) {
            pclose(fp);
            return;
        }
    }
    pclose(fp);
    
    // Remove DHCP service
    system("pkexec firewall-cmd --remove-service=dhcp --permanent >/dev/null 2>&1");
    system("pkexec firewall-cmd --remove-service=dhcp >/dev/null 2>&1");
}