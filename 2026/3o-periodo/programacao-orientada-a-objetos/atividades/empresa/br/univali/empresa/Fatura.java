package br.univali.empresa;

public class Fatura implements Pagavel {
    public float valor;

    public Fatura(float valor) {
        this.valor = valor;
    }

    @Override
    public float getValor() {
        return valor;
    }
}
