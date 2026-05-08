package br.univali.empresa;

public class Horista extends Funcionario {
    public float horasTrabalhadas;
    public float valorHora;

    public Horista(String nome, int idade, String setor, float horasTrabalhadas, float valorHora) {
        super(nome, idade, setor);
        this.horasTrabalhadas = horasTrabalhadas;
        this.valorHora = valorHora;
    }

    @Override
    public float getSalario() {
        return horasTrabalhadas * valorHora;
    }
}
