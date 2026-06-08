package br.univali.empresa;

public class Assalariado extends Funcionario {
    public float salarioFixo;

    public Assalariado(String nome, int idade, String setor, float salarioFixo) {
        super(nome, idade, setor);
        this.salarioFixo = salarioFixo;
    }

    @Override
    public float getSalario() {
        return salarioFixo;
    }
}
