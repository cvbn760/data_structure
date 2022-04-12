#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "data_structure/tree.h"
#include "data_structure/tree.c"
#include <stdio.h>
#include <stdlib.h>
#define _OPEN_SYS_ITOA_EXT
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>


typedef int8_t              INT8;     /*  8-bit signed integer, used for numeric data */
typedef int16_t             INT16;    /* 16-bit signed integer */
typedef int32_t             INT32;    /* 32-bit signed integer */
typedef int64_t             INT64;    /* 64-bit signed integer */

typedef uint8_t             UINT8;    /*  8-bit unsigned integer, used for numeric data */
typedef uint16_t            UINT16;   /* 16-bit unsigned integer */
typedef uint32_t            UINT32;   /* 32-bit unsigned integer */
typedef uint64_t            UINT64;   /* 64-bit unsigned integer */


typedef struct {
    UINT32 utc;			// timestamp UTC, секунды
    INT32 latitude;		// широта в угловых мС
    INT32 longitude;	// долгота в угловых мС
    float hdop;			// горизонтальная точность HDOP
    float altitude;		// высота над уровнем моря, метры
    char fix;			// признак достоверности геоданных
    float course;		// курс (угол), градусы.минуты
    UINT16 speed;		// скорость, км/ч
    UINT8 gps_sat;		// кол-во спутников GPS
    UINT8 glonass_sat;	// кол-во спутников GPS
} gnss_data;


void clearString(char *str){
    memset(str,0,strlen(str));
    free(str);
    str = NULL;
}

char** str_split(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;
    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }
    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);
    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;
    result = malloc(sizeof(char*) * count);
    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);
        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }
    return result;
}

char* substr(const char *src, int m, int n){
    // get the length of the destination string
    int len = n - m;

    // allocate (len + 1) chars for destination (+1 for extra null character)
    char *dest = (char*)malloc(sizeof(char) * (len + 1));

    // extracts characters between m'th and n'th index from source string
    // and copy them into the destination string
    for (int i = m; i < n && (*(src + i) != '\0'); i++)
    {
        *dest = *(src + i);
        dest++;
    }

    // null-terminate the destination string
    *dest = '\0';

    // return the destination string
    return dest - len;
}


// 1656922855
// 1649308855
//      hhmmss.sssddmmyy
//<UTC>[082055.000070422]
UINT32 getUtcFromString(char *str){
    if (strlen(str) != 16){
        // Неверная длина строки
        return 0;
    }
    struct tm m_time;
    char *subst;

    // int часы (отсчет с 0);
    subst = substr(str, 0,2);
    m_time.tm_hour = atoi(subst);
    clearString(subst);

    // int минуты (отсчет с 0);
    subst = substr(str, 2,4);
    m_time.tm_min = atoi(subst);
    clearString(subst);

    // int секунды (отсчет с 0);
    subst = substr(str, 4,6);
    m_time.tm_sec = atoi(subst);
    clearString(subst);

    // int день месяца (отсчет с 1);
    subst = substr(str, 10,12);
    m_time.tm_mday = atoi(subst) + 1;
    clearString(subst);

    // int месяц (отсчет с 0);
    subst = substr(str, 12,14);
    m_time.tm_mon = atoi(subst) - 1;
    clearString(subst);

    // int год (за начала отсчета принят 1900 год);
    char* z = substr(str, 14,16);
    subst = strconc("20",z);
    m_time.tm_year = atoi(subst) - 1900;
    clearString(subst);
    clearString(z);


    m_time.tm_wday = 0;   // день недели (воскресенье - 0);
    m_time.tm_yday = 0;   // день в году (отсчет с 0);
    m_time.tm_isdst = -1; // признак "летнее время"
    return mktime (&m_time);
}

double degreeToMilSec(double degrees, double minutes){
    // Конвертация из представления широты/долготы градусы, минуты, доли минут в градусы и доли градусов
    double result = degrees + (minutes / 60);
    // Переводим градусы в секунды
    // 1 угловой градус - 3600 угловых секунд или (3600 * 1000) милисекунд
    result = result * 3600 * 1000;
    return result;
}


//5332.2471N
//ddmm.mmmm N/S
//N/S => +/-
INT32 getLat(char *str){
    if (strlen(str) != 10){
        // Некорректная строка
        return 0;
    }
    // Получаем градусы
    char *y;
    char *content = substr(str, 0,2);
    double degrees = strtod(content, &y);
    clearString(content);

    // Получаем минуты
    content = substr(str,2,9);
    char *x;
    double minutes = strtod(content,&x);
    clearString(content);
    double result = degreeToMilSec(degrees, minutes);
    if(str[9] == 'S'){
        result *= (-1);
    }
    return result;
}

//04918.9815E
//dddmm.mmmm E/W
//E/W => +/-
INT32 getLon(char *str){
    if (strlen(str) != 11){
        // Некорректная строка
        return 0;
    }

    // Получаем градусы
    char *y;
    char *content = substr(str, 0,3);
    double degrees = strtod(content, &y);
    clearString(content);

    // Получаем минуты
    content = substr(str,3,10);
    char *x;
    double minutes = strtod(content,&x);
    clearString(content);

    double result = degreeToMilSec(degrees, minutes);

    if(str[10] == 'W'){
        result *= (-1);
    }
    return result;
}

char *validateBeforeSplit(char *string, const char delim, char *addon){
    char *prev;
    size_t addonLen = strlen(addon);
    char *after;
    char *dub = strdup(string);
    int len = strlen(dub);
    for (int i = 0; i < len + 1; ++i) {
        if (dub[i] == delim){
            if (dub[i+1] == delim || dub[i+1] == '\0' || i == 0){
                if (i == 0){
                    prev = multConc(1, "");
                    after = substr(dub,i,(len));
                } else{
                    prev = substr(dub, 0, i + 1);
                    after = substr(dub,i + 1,len);
                }
                clearString(dub);
                dub = multConc(3, prev, addon, after);
                i = i + addonLen - 1;
                clearString(prev);
                clearString(after);
                len = strlen(dub);
            }
        }
    }
    return dub;
}

gnss_data parse_gnss_mess(char *message) {
    if (message == NULL || strcmp(message, "ERROR") == 0){
        printf("Err\n");
        message = ",,,,,,,,,,";
    }
    message = validateBeforeSplit(message, ',',"0");
    gnss_data gnssData;
    char **tokens;
    char fx;
    tokens = str_split(message, ',');
    if (tokens) {
        char *dateTime;
        int i;
        for (i = 0; *(tokens + i); i++) {
            switch (i) {
                case 0:  // <UTC> part1
                    dateTime = malloc(10+1);
                    strcpy(dateTime, *(tokens + i));
                    break;
                case 1:  // <latitude>
                    gnssData.latitude = getLat(*(tokens + i));
                    break;
                case 2:  // <longitude>
                    gnssData.longitude = getLon(*(tokens + i));
                    break;
                case 3:  // <hdop>
                    gnssData.hdop = atof(*(tokens + i));
                    break;
                case 4:  // <altitude>
                    gnssData.altitude = atof(*(tokens + i));
                    break;
                case 5:  // <fix>
                    fx = *(tokens + i)[0];
                    if(fx == '0' || fx == '1' || fx == '2' || fx == '3'){
                        gnssData.fix = fx;
                    } else{
                        gnssData.fix = '0';
                    }
                    break;
                case 6:  // <cog>
                    gnssData.course = atof(*(tokens + i));
                    break;
                case 7:  // <spkm>
                    gnssData.speed = atoi(*(tokens + i));
                    break;
                case 8:  // <spkn> not use
                    break;
                case 9:  // <date> part2
                    dateTime = strconc(dateTime,*(tokens + i));
                    gnssData.utc = getUtcFromString(dateTime);
                    free(dateTime);
                    break;
                case 10: // <nsat_gps>
                    gnssData.gps_sat = atoi(*(tokens + i));
                    break;
                case 11: // <nsat_glonass>
                    gnssData.glonass_sat = atoi(*(tokens + i));
                    break;
                default:
                    break;
            }
            free(*(tokens + i));
        }
        if (i != 12){
            goto down;
        }
        free(tokens);
        return gnssData;
    } else{
        down:
        gnssData.utc = 0;
        gnssData.latitude = 0;
        gnssData.longitude = 0;
        gnssData.fix = '0';
        gnssData.course = 0;
        gnssData.speed = 0;
        gnssData.hdop = 0;
        gnssData.glonass_sat = 0;
        gnssData.gps_sat = 0;
        gnssData.altitude = 0;
        return gnssData;
    }
}

//char *normalizeMessage(char *string, const char delim){
//    char *prev;
//    char *addon = " ";
//    char *after;
//    char *dub = strdup(string);
//    int len = strlen(dub);
//    for (int i = 0; i < len + 1; ++i) {
//        if (dub[i] == delim){
//            if (dub[i+1] == delim){
//                if (i == 0){
//                    prev = multConc(1, "");
//                    after = substr(dub,i,(len));
//                } else{
//                    prev = substr(dub, 0, i + 1);
//                    after = substr(dub,i + 1,(len-1));
//                }
//                clearString(dub);
//                dub = multConc(3, prev, addon, after);
//                len = strlen(dub) + 1;
//                clearString(prev);
//                clearString(after);
//            }
//        }
//    }
//    return dub;
//}

int main(void) {
    char str[] = "test,test.test,test.test,test,test,test,test,test,test,test,test,test";
    gnss_data gn = parse_gnss_mess("082055.000,5332.2471N,04918.9815E,5.6,107.5,2,355.2,1.9,1.0,070422,01,02");

    printf("time %d\n", gn.utc);
    printf("latitude %d\n", gn.latitude);
    printf("longitude %d\n", gn.longitude);
    printf("hdop %f\n", gn.hdop);
    printf("altitude %f\n", gn.altitude);
    printf("fix %c\n", gn.fix);
    printf("course %f\n", gn.course);
    printf("speed %d\n", gn.speed);
    printf("gps_sat %d\n", gn.gps_sat);
    printf("glonas_sat %d\n", gn.glonass_sat);
    return 0;
}


