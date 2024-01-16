#include <stdio.h>

int main()
{
    const char *dateString = "13-01-2004";
    int day, month, year;

    // Use sscanf to parse the string
    sscanf(dateString, "%d-%d-%d", &day, &month, &year);

    // Print the parsed values
    printf("Year: %d\n", year);
    printf("Month: %d\n", month);
    printf("Day: %d\n", day);
    return 0;
}
