package br.univali.cc.banco;

import java.util.ArrayList;
import java.util.List;

public class ContaCorrente {
    final private boolean especial;
    final private double limite;
    final private int numero;
    private double saldo;
    final private List<Movimentacao> movimentacoes;

    public ContaCorrente(int numero, double saldoInicial) {
        this.numero = numero;
        this.saldo = saldoInicial;
        this.especial = false;
        this.limite = 0.0;
        this.movimentacoes = new ArrayList<>();
        criarMovimentacao("depósito inicial", 'C', saldoInicial);
    }

    public ContaCorrente(int numero, double saldoInicial, double limite) {
        this.numero = numero;
        this.saldo = saldoInicial;
        this.especial = true;
        this.limite = limite;
        this.movimentacoes = new ArrayList<>();
        criarMovimentacao("depósito inicial", 'C', saldoInicial);
    }

    private void criarMovimentacao(String descricao, char tipo, double valor) {
        movimentacoes.add(new Movimentacao(descricao, tipo, valor));
    }

    protected boolean depositar(double valor) {
        if (valor <= 0)
            return false;
        saldo += valor;
        criarMovimentacao("depósito", 'C', valor);
        return true;
    }

    public String emitirExtrato() {
        StringBuilder sb = new StringBuilder();
        sb.append("======================================\n");
        sb.append("  extrato - conta nº ").append(numero).append("\n");
        sb.append("  tipo: ").append(especial ? "especial" : "normal").append("\n");
        if (especial)
            sb.append("  limite: R$ ").append(String.format("%.2f", limite)).append("\n");
        sb.append("======================================\n");
        for (Movimentacao mov : movimentacoes) {
            sb.append("  ").append(mov.getMovimentacao()).append("\n");
        }
        sb.append("--------------------------------------\n");
        sb.append("  saldo atual: R$ ").append(String.format("%.2f", saldo)).append("\n");
        sb.append("======================================\n");
        return sb.toString();
    }

    public int getNumeroConta() {
        return numero;
    }

    public double getSaldo() {
        return saldo;
    }

    protected boolean sacar(double valor) {
        if (valor <= 0)
            return false;
        if (valor > saldo + limite) {
            System.out.println("saldo insuficiente. disponível: R$ " + String.format("%.2f", saldo + limite));
            return false;
        }
        saldo -= valor;
        criarMovimentacao("saque", 'D', valor);
        return true;
    }
}
