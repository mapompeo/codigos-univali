package br.univali.cc.financas;

import java.util.ArrayList;
import java.util.Date;
import java.util.List;

public class Aplicacao {
    private List<Gasto> gastos = new ArrayList<>();
    private List<TipoGasto> tipos = new ArrayList<>();
    private List<FormaPagamento> formas = new ArrayList<>();

    public void adicionarFormaPagamento(FormaPagamento forma) {
        formas.add(forma);
    }

    public void adicionarTipoGasto(TipoGasto tipo) {
        tipos.add(tipo);
    }

    public void listarGastos(int mes, int ano) {

    }

    public void novoGasto(String descricao, Date data, Double valor, TipoGasto tipo, FormaPagamento forma) {
        return;
    }
}
