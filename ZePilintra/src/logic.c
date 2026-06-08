#include "logic.h"
#include "raylib.h"

static LogicQuestion questions[] =
{
    {"p v ~p", LOGIC_TAUTOLOGY},
    {"p ^ ~p", LOGIC_CONTRADICTION},
    {"p -> q", LOGIC_CONTINGENCY},
    {"(p ^ q) -> p", LOGIC_TAUTOLOGY},
    {"(p v q) ^ ~(p v q)", LOGIC_CONTRADICTION},
    {"(p -> q) ^ (q -> p)", LOGIC_CONTINGENCY}
};

LogicQuestion GetRandomQuestion(void)
{
    int count = sizeof(questions) / sizeof(questions[0]);

    return questions[GetRandomValue(0, count - 1)];
}