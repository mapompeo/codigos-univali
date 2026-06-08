package br.univali.empresa;

public class PrestadorServico implements Pagavel {
    private float valor;

    public PrestadorServico(float valor) {
        this.valor = valor;
    }

    @Override
    public float getValor() {
        return valor;
    }
}
