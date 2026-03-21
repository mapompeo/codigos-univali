#include <iostream>
#include "raylib.h"

using namespace std;

struct No
{
    int data;
    No *next;
    No *prev;
};

struct Deque
{
    No *first;
    No *last;
    int count;
};

No *CreateNo(int valor)
{
    No *newNo = (No *)calloc(1, sizeof(No));

    newNo->data = valor;
    newNo->next = NULL;
    newNo->prev = NULL;

    return newNo;
}

void PushFront(Deque *d, int value)
{
    No *newNo = CreateNo(value);

    if (d->first == NULL)
    {
        d->first = newNo;
        d->last = newNo;
    }
    else
    {
        newNo->next = d->first;
        d->first->prev = newNo;
        d->first = newNo;
    }

    d->count++;
}

void PushBack(Deque *d, int value)
{
    No *newNo = CreateNo(value);

    if (d->first == NULL)
    {
        d->first = newNo;
        d->last = newNo;
    }
    else
    {
        newNo->prev = d->last;
        d->last->next = newNo;
        d->last = newNo;
    }

    d->count++;
}

int main()
{
    InitWindow(1000, 500, "Aula - 07 - Filas");
    Deque *deque = new Deque();
    int value = 1;
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_Q))
            PushFront(deque, value++);
        if (IsKeyPressed(KEY_W))
            PushBack(deque, value++);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText("TESTE", 20, 20, 20, DARKGRAY);
        DrawText("[Q] Push Front - [W] Push Back", 20, 50, 18, RED);
        DrawText(TextFormat("Total de nós: %d", deque->count), 20, 110, 20, DARKGRAY);

        No *curr = deque->first;
        int i = 0;
        while (curr != NULL)
        {
            int x = 100 + (i * 130);
            int y = 250;

            Rectangle rec = {x, y, 90, 60};
            DrawRectangleRec(rec, YELLOW);
            DrawRectangleLinesEx(rec, 2, GREEN);
            DrawText(TextFormat("%02d", curr->data), x + 35, y + 20, 25, BLACK);

            if (curr->next != NULL)
            {
                DrawLineEx((Vector2){x + 90, y + 20}, (Vector2){x + 130, y + 20}, 2, BLUE);
                DrawTriangle((Vector2){x + 135, y + 20}, (Vector2){x + 125, y + 15}, (Vector2){x + 125, y + 25}, BLUE);

                DrawLineEx((Vector2){x + 130, y + 40}, (Vector2){x + 90, y + 40}, 2, DARKPURPLE);
                DrawTriangle((Vector2){x + 85, y + 40}, (Vector2){x + 95, y + 45}, (Vector2){x + 95, y + 35}, DARKPURPLE);
            }

            if (curr == deque->first)
                DrawText("Primeiro", x + 25, y - 25, 15, BLUE);
            if (curr == deque->last)
                DrawText("Ultimo", x + 25, y + 70, 15, ORANGE);

            curr = curr->next;
            i++;
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}