#include <pwd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sysinfo.h>
#include <sys/utsname.h>

char* tux_ascii =
        " ------------------------------\n"
        "      \\\n"
        "       \\\n"
        "           .--.\n"
        "          |o_o |\n"
        "          |:_/ |\n"
        "         //   \\ \\\n"
        "        (|     | )\n"
        "       /'\\_   _/`\\\n"
        "       \\___)=(___/\n\n";

char* get_kernel_release() {
    FILE *fp = fopen("/proc/version", "r");
    char* buffer = NULL;
    size_t len;
    ssize_t bytes_read = getdelim( &buffer, &len, '\0', fp);

    char *kernel_release = strtok(buffer, " ");
    for(int i=0;2>i;i++)
        kernel_release = strtok(NULL, " ");
    
    return kernel_release;
}

char* get_shell() {
    FILE *fp = fopen("/etc/passwd", "r");
    char* buffer = NULL;
    size_t len;
    ssize_t bytes_read = getdelim( &buffer, &len, '\0', fp);

    uid_t uid = geteuid();
    struct passwd *pw = getpwuid(uid);
    char *username = pw->pw_name;

    char *shell = strtok(buffer, "\n");
    while(shell != NULL) {
        if (strstr(shell, username) != NULL) break;
        shell = strtok(NULL, "\n");
    }

    shell = strtok(shell, ":");
    for(int i=0;5>i;i++)
        shell = strtok(NULL, ":");

    return shell;
}

char* get_os_name() {
    FILE *fp = fopen("/etc/os-release", "r");

    char* buffer = NULL;
    size_t len;
    ssize_t bytes_read = getdelim( &buffer, &len, '\0', fp);

    char *os_name = strtok(buffer, "=\"");
    for(int i=0;3>i;i++)
        os_name = strtok(NULL, "=\"");
    
    os_name = strtok(os_name, "\n");

    struct utsname unameData;
    uname(&unameData);

    char *os_name_full = (char*)malloc(13 * sizeof(char));
    sprintf(os_name_full, "%s %s", os_name, unameData.machine);

    return os_name_full;
}

char* get_uptime() {
    struct sysinfo s_info;
    sysinfo(&s_info);

    long uptime_s = s_info.uptime;
    
    long minutesInSeconds = 60;
    long hoursInSeconds = minutesInSeconds * 60;
                          
    long elapsedHours = uptime_s / hoursInSeconds;
    uptime_s = uptime_s % hoursInSeconds;

    long elapsedMinutes = uptime_s / minutesInSeconds;
    uptime_s = uptime_s % minutesInSeconds;

    char *uptime = (char*)malloc(13 * sizeof(char));
    sprintf(uptime, "%dh %dm %ds", elapsedHours, elapsedMinutes, uptime_s);

    return uptime;
}

char* get_ram_usage() {
    struct sysinfo s_info;
    sysinfo(&s_info);

    long total_RAM = s_info.totalram / 1024 / 1024;
    long used_RAM = total_RAM - s_info.freeram / 1024 / 1024;

    char *ram_usage = (char*)malloc(13 * sizeof(char));
    sprintf(ram_usage, "%dM / %dM", used_RAM, total_RAM);

    return ram_usage;
}

int main() {
    printf("\n ------------------------------\n");

    printf("     \x1B[35mos\x1B[0m %s \n", get_os_name());
    printf("     \x1B[35mkernel\x1B[0m %s \n", get_kernel_release());
    printf("     \x1B[35muptime\x1B[0m %s \n", get_uptime());
    printf("     \x1B[35mshell\x1B[0m %s \n", get_shell());
    printf("     \x1B[35mmemory\x1B[0m %s \n", get_ram_usage());

    printf(tux_ascii);

    return 0;
}