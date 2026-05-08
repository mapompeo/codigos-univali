package br.univali.empresa;

public abstract class Funcionario implements Pagavel {
    public String nome;
    public int idade;
    public String setor;

    public Funcionario(String nome, int idade, String setor) {
        this.nome = nome;
        this.idade = idade;
        this.setor = setor;
    }

    public String getSetor() {
        return setor;
    }

    public abstract float getSalario();

    @Override
    public float getValor() {
        return getSalario();
    }
}
