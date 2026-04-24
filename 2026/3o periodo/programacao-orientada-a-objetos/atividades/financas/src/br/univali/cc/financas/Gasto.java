package br.univali.cc.financas;

import java.util.Date;

public class Gasto {
    private Date data;
    private String descricao;
    private Double valor;
    private TipoGasto tipo;
    private FormaPagamento forma;

    public Gasto(Date data, String descricao, double valor, TipoGasto tipo, FormaPagamento forma) {
        this.data = data;
        this.descricao = descricao;
        this.valor = valor;
        this.tipo = tipo;
        this.forma = forma;
    }

    public String obterDescricao() {
        return descricao + " | Tipo: " + tipo.getDescricao() + " | Forma: " + forma.getDescricao();
    }
}
