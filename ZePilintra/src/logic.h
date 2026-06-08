#ifndef LOGIC_H
#define LOGIC_H

typedef enum
{
    LOGIC_TAUTOLOGY,
    LOGIC_CONTRADICTION,
    LOGIC_CONTINGENCY

} LogicQuestionType;

typedef struct
{
    char proposition[100];
    LogicQuestionType answer;

} LogicQuestion;

LogicQuestion GetRandomQuestion(void);

#endif