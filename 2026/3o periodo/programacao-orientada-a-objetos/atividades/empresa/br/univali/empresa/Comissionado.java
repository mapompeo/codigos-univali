package br.univali.empresa;

public class Comissionado extends Funcionario {
    public float salarioBase;
    public float comissao;

    public Comissionado(String nome, int idade, String setor, float salarioBase, float comissao) {
        super(nome, idade, setor);
        this.salarioBase = salarioBase;
        this.comissao = comissao;
    }

    @Override
    public float getSalario() {
        return salarioBase + comissao;
    }
}
