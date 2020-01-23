#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>

int hourLocal;
int minuteLocal;
struct movie {
    char hallName[20];
    int hourStart;
    int minuteStart;
    int hourEnd;
    int minuteEnd;
    char movieName[20];
    char movieDetail[60];
    int freeSpace;
};
struct node {
    struct movie data;
    struct node *next;
};

struct node *
createNode(int hourS, int minuteS, int hourE, int minuteE, char name[], char detail[], int space, char hallName[]) {
    struct node *nn;
    nn = (struct node *) malloc(sizeof(struct node));
    nn->data.hourStart = hourS;
    nn->data.minuteStart = minuteS;
    nn->data.hourEnd = hourE;
    nn->data.minuteEnd = minuteE;
    strcpy(nn->data.hallName, hallName);
    strcpy(nn->data.movieName, name);
    nn->data.freeSpace = space;
    strcpy(nn->data.movieDetail, detail);
    nn->next = NULL;
    return nn;
}

void insert(struct node **head, struct node *createNode) {
    if (*head == NULL) {
        *head = createNode;
        return;
    }

    struct node *curr;
    curr = *head;
    while (curr->next != NULL)
        curr = curr->next;
    curr->next = createNode;
}

void deleteNode(struct node **head, char movie[]) {
    if (*head == NULL) {
        return;
    }

    if (strcmp((*head)->data.movieName, movie) == 0) {
        struct node *temp;
        temp = *head;
        (*head) = (*head)->next;
        free(temp);
        printf("'%s' has been removed\n", movie);
        return;
    }

    struct node *search = *head;
    while (search->next != NULL) {
        if (strcmp((*head)->data.movieName, movie) == 0) {
            struct node *temp;
            temp = search->next;
            search->next = search->next->next;
            free(temp);
            printf("'%s' has been removed\n", movie);
            return;
        }
        search = search->next;
    }
    printf("'%s' does not exist\n", movie);

}

void printList(struct node *current) {
    int i = 1;
    while (current != NULL) {
        if (current->data.hourStart < hourLocal ||
            (current->data.minuteStart < minuteLocal && current->data.hourStart == hourLocal))
            current = current->next;

        printf("%d)\nStart : %d:%d\nEnd : %d:%d\nMovie name : %s\nMovie detail : %s\nHall name: %s\nTickets left : %d\n",
               i,
               current->data.hourStart, current->data.minuteStart, current->data.hourEnd, current->data.minuteEnd,
               current->data.movieName,
               current->data.movieDetail, current->data.hallName, current->data.freeSpace);

        i++;

        current = current->next;
    }
}

int timeAsMinute(int hour, int minute) {
    return hour * 60 + minute;
}

void sort(struct node **head) {
    if ((*head) == NULL || (*head)->next == NULL) return;
    struct node *i, *j;
    for (i = *head; i != NULL; i = i->next) {
        for (j = i->next; j != NULL; j = j->next) {
            if (timeAsMinute(i->data.hourStart, i->data.minuteStart) >
                timeAsMinute(j->data.hourStart, j->data.minuteStart)) {
                struct movie temp;
                temp = i->data;
                i->data = j->data;
                j->data = temp;
            }
        }
    }
}


void showFile(char filename[]) {
    FILE *fptr;

    fptr = fopen(filename, "r");
    if (fptr == NULL) {
        printf("Cannot open file \n");
        return;
    }
    while (1) {
        char c = fgetc(fptr);
        if (feof(fptr)) break;
        printf("%c", c);

    }

    fclose(fptr);
}

int intersectionLen(int startA, int startB, int endA, int endB) {
    int totalEnd = fmin(endA, endB);
    int totalStart = fmax(startA, startB);
    return fmax(0, totalEnd - totalStart);
}

bool hasInterference(struct node *head, char hallName[], int startHour, int startMinute, int endHour, int endMinute) {
    int startTimeByMinute = timeAsMinute(startHour, startMinute);
    int endTimeByMinute = timeAsMinute(endHour, endMinute);
    struct node *curr;
    for (curr = head; curr != NULL; curr = curr->next) {
        if (strcmp(hallName, curr->data.hallName) != 0)
            continue;
        if (intersectionLen(timeAsMinute(curr->data.hourStart, curr->data.minuteStart), startTimeByMinute,
                            timeAsMinute(curr->data.hourEnd, curr->data.minuteEnd), endTimeByMinute) != 0) {
            return true;
        }
    }
    return false;
}

int main() {
    struct node *head = NULL;
    time_t rawtime;
    struct tm *timeinfo;
    char hallName[20];
    FILE *fPtr;
    int movieCnt = 0;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    printf("Current local time and date: %s\n", asctime(timeinfo));
    time_t now;
    struct tm *now_tm;
    fPtr = fopen("movies.txt", "r");
    if (fPtr != NULL) {
        int hourS, minuteS, tickets, hourE, minuteE, i;
        char movieName[20], movieDetail[60];
        fscanf(fPtr,
               "%d)\nStart : %d:%d\nEnd : %d:%d\nMovie name : %s\nMovie detail : %s\nMovie hall name : %s\nMovie Tickets left : %d\n",
               &i,&hourS, &minuteS, &hourE,&minuteE,movieName, movieDetail, hallName,&tickets);
        insert(&head,createNode(hourS,minuteS,hourE,minuteE,movieName,movieDetail,tickets,hallName));
    }
    fclose(fPtr);
    now = time(NULL);
    now_tm = localtime(&now);
    hourLocal = now_tm->tm_hour;
    minuteLocal = now_tm->tm_min;
    printf("main menu :\n");
    while (1) {
        printf("1) Add a movie\n2) Reserve a movie\n3) Show list\n4) Close page\n");
        int n;
        scanf("%d", &n);
        if (n == 1) {
            printf("Please enter the Hall name : ");
            scanf("%s", hallName);
            int hourS, minuteS, tickets, hourE, minuteE;
            char movieName[20], movieDetail[60];
            printf("Please enter the movie name : ");
            scanf("%s", movieName);
            printf("Please enter the movie detail : ");
            scanf("%s", movieDetail);
            printf("Please enter the movie START time : ");
            scanf("%d:%d", &hourS, &minuteS);
            printf("Please enter the movie END time : ");
            scanf("%d:%d", &hourE, &minuteE);
            printf("Please enter how many tickets you have : ");
            scanf("%d", &tickets);
            if (hasInterference(head, hallName, hourS, minuteS, hourE, minuteE)) {
                printf("There is another movie playing!\n");
                continue;
            }

            insert(&head, createNode(hourS, minuteS, hourE, minuteE, movieName, movieDetail, tickets, hallName));
            sort(&head);

            struct node *current = head;
            int i = 1;
            fPtr = fopen("movies.txt", "w");
            while (current != NULL) {

                fprintf(fPtr,
                        "%d)\nStart : %d:%d\nEnd : %d:%d\nMovie name : %s\nMovie detail : %s\nMovie hall name : %s\nMovie Tickets left : %d\n",
                        i,
                        current->data.hourStart, current->data.minuteStart, current->data.hourEnd,
                        current->data.minuteEnd,
                        current->data.movieName, current->data.movieDetail, current->data.hallName,
                        current->data.freeSpace);
                current = current->next;
                i++;

            }
            fclose(fPtr);
            movieCnt++;

        }
        if (n == 2) {
            printf("Please select a movie : \n");
            printf("%d\n", movieCnt);
            printList(head);
            int x;
            scanf("%d", &x);
            if (x < 0 || x > movieCnt) {
                printf("Movie not found!\n");
                continue;
            }
            struct node *curr = head;
            for (int i = 0; i < x - 1; i++)
                curr = curr->next;
            if (curr->data.freeSpace > 0) {
                curr->data.freeSpace--;
                printf("Reservation completed!\n");
            } else {
                printf("There is no room left!\n");
            }

        }
        if (n == 3) {
            printList(head);
        }
        if (n == 4) return 0;

        printf("\n");
    }


    return 0;
}
