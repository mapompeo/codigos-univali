package br.univali.cc.banco;

public class Movimentacao {
    private String descricao;
    private char tipo; // 'c' = crédito, 'd' = débito
    private double valor;

    public Movimentacao(String descricao, char tipo, double valor) {
        this.descricao = descricao;
        this.tipo = tipo;
        this.valor = valor;
    }

    public String getMovimentacao() {
        String tipoStr = (tipo == 'C') ? "Crédito" : "Débito";
        return descricao + " | Tipo: " + tipoStr + " | Valor: R$ " + String.format("%.2f", valor);
    }
}
