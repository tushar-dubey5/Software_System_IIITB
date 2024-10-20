#include <stdlib.h>
#include <string.h>
#include "feedback.h"

#define MAX_FEEDBACKS 100

static Feedback feedbacks[MAX_FEEDBACKS];
static int feedback_count = 3;

// Initial mock data
static Feedback initial_feedbacks[] = {
    {1, 101, 5, "Great service!"},
    {2, 102, 4, "Good experience overall."},
    {3, 103, 3, "Average service."}
};

// Initialize the feedbacks array with the initial mock data
__attribute__((constructor)) static void init_feedbacks() {
    memcpy(feedbacks, initial_feedbacks, sizeof(initial_feedbacks));
}

int get_all_feedback(Feedback** feedback_list) {
    *feedback_list = malloc(feedback_count * sizeof(Feedback));
    if (*feedback_list == NULL) {
        return 0;
    }

    for (int i = 0; i < feedback_count; i++) {
        (*feedback_list)[i] = feedbacks[i];
    }

    return feedback_count;
}

int add_feedback(const Feedback *feedback) {
    if (feedback_count >= MAX_FEEDBACKS) {
        return 0; // Feedback storage is full
    }

    feedbacks[feedback_count] = *feedback;
    feedbacks[feedback_count].id = feedback_count + 1; // Assign an incremental ID
    feedback_count++;

    return 1; 
}