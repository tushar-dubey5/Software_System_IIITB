#ifndef FEEDBACK_H
#define FEEDBACK_H

#define MAX_COMMENT_LENGTH 256

typedef struct {
    int id;
    int user_id;
    int rating;
    char comment[MAX_COMMENT_LENGTH];
} Feedback;

int get_all_feedback(Feedback** feedbacks);
int add_feedback(const Feedback *feedback);

#endif