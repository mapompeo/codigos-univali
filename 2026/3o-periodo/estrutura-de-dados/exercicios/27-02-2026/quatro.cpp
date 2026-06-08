#include <iostream>
#include <stdio.h>
using namespace std;

main()
{
    int notaAluno, quantidaDeAluno, acimaDesete;

    cout << "Insira a quantidade de alunos: ";
    cin >> quantidaDeAluno;

    for (int i = 0; i < quantidaDeAluno; i++)
    {
        cout << "Insira a nota do aluno " << quantidaDeAluno << ": ";
        cin >> notaAluno;
        if (notaAluno > 7)
        {
            acimaDesete++;
        }
    }

    cout << "A média da turma é " << notaAluno / quantidaDeAluno;
    cout << "A quantidade de alunos acima de sete é " << acimaDesete;
}