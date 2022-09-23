// Implements binary search for sorted numbers
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int binarySearch(char *dates[], int size, char *searchDate);
int bisectLeft(char *dates[], int size, char *searchDate);
int bisectLeft2(char *dates[], int size, char *searchDate);
int bisectRight(char *dates[], int size, char *searchDate);

int main(void) {
    // An array of dates
    char *dates[] = {
        "2022-01-01 00:00:00",
        "2022-02-02 13:00:00",
        "2022-03-03 14:59:59",
        "2022-04-04 15:00:00",
        "2022-05-05 16:00:00",
        "2022-06-06 17:00:00",
        "2022-06-07 18:00:00",
        "2022-06-08 19:00:00",
        "2022-06-18 20:00:00",
        "2022-06-18 23:00:00",
        "2022-06-19 09:00:00",
    };

    char date[] = "2022-06-09 17:00:00";
    int length = sizeof(dates) / sizeof dates[0];

    printf("length: %i\n", length);

    int index = bisectLeft(dates, length, date);

    printf("%s found at index %i\n\n", date, index);
    for (int i = index; i < length; i++) {
        printf("%s\n", dates[i]);
    }

    return 0;
}

time_t strToTime(const char *str) {
    time_t date = -1;
    struct tm tm;
    if (strptime(str, "%Y-%m-%d %H:%M:%S", &tm)) {
        date = mktime(&tm);
    }
    return date;
}
// https://leetcode.com/problemset/all/
// https://stackoverflow.com/questions/2260272/how-to-find-the-last-element-of-array-in-binary-search
// https://leetcode.com/problems/find-first-and-last-position-of-element-in-sorted-array/discuss/702162/python-lets-implement-pythons-bisect-algorithm
int bisectLeft(char *dates[], int size, char *searchDate) {
    int low = 0;
    int steps = 0;
    int high = size - 1;
    int mid;

    time_t searchDateTime = strToTime(searchDate);
    time_t lowTime = strToTime(dates[low]);
    time_t highTime = strToTime(dates[high]);

    if (searchDateTime < lowTime) {
        return low;
    } else if (searchDateTime >= highTime) {
        return high;
    }

    while (low < high) {
        steps++;
        mid = (low + high) / 2;
        time_t midTime = strToTime(dates[mid]);
        printf("mid: %i\n", mid);
        if (searchDateTime > midTime) {
            low = mid + 1;
        } else {
            high = mid;
        }
        printf("low: %i\nhigh: %i\n", low, high);
    }

    printf("steps: %i\n", steps);

    return low;
}

// https://gist.github.com/CMCDragonkai/ad36cd5f224d9390236a
// https://github.com/python/cpython/blob/main/Lib/bisect.py
int bisectLeft2(char *dates[], int size, char *searchDate) {
    int low = 0;
    int steps = 0;
    int high = size - 1;
    int mid;

    time_t searchDateTime = strToTime(searchDate);
    time_t lowTime = strToTime(dates[low]);
    time_t highTime = strToTime(dates[high]);

    if (searchDateTime < lowTime) {
        return 0;
    } else if (searchDateTime >= highTime) {
        return size - 1;
    }

    while ((high - low) > 1) {
        steps++;
        mid = (low + high) / 2;
        time_t midTime = strToTime(dates[mid]);
        printf("mid: %i\n", mid);
        printf("(high(%i) - low(%i)): %i\n", high, low, (high - low));
        if (searchDateTime <= midTime) {
            high = mid;
        } else {
            low = mid;
        }
    }

    printf("steps: %i\n", steps);

    return high;
}

/**
 * Binary search for sorted numbers (dates)  in an array of dates
 *
 *
 * @param dates Array of dates
 * @param size Size of array
 * @param searchDate Date to search for
 *
 * @return int Index of date in array
 */

int binarySearch(char *dates[], int size, char *searchDate) {
    int low = 0;
    int steps = 0;
    int high = size - 1;
    int mid;

    if (low > high) {
        return -1;
    }
    time_t searchDateTime = strToTime(searchDate);

    while (low <= high) {
        steps++;
        mid = (low + high) / 2;
        time_t midTime = strToTime(dates[mid]);
        printf("mid: %i\n", mid);
        if (midTime == searchDateTime) {
            return mid;
        } else if (searchDateTime > midTime) { // date is on the right side
            low = mid + 1;
            printf("low: %i\n", low);
        } else { // date is on the left side
            high = mid - 1;
            printf("high: %i\n", high);
        }
    }

    printf("steps: %i\n", steps);

    return low;
}