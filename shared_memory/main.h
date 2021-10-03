#define MAX_NUMBER_OF_COMPANIES 50
#define MAX_LEMGTH_OF_COMPANY_NAME 20
#define MONITOR_WAIT_TIME 1000

struct company {
    char name[MAX_LEMGTH_OF_COMPANY_NAME];
    int work_done;
};

struct company_registry {
    struct company companies[MAX_NUMBER_OF_COMPANIES];
};
