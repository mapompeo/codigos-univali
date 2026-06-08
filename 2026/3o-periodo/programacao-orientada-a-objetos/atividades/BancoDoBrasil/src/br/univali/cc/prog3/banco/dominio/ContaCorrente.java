package br.univali.cc.prog3.banco.dominio;

import java.util.ArrayList;
import java.util.List;

public class ContaCorrente {
    private final boolean especial;
    private final double limite;
    private final int numero;
    private double saldo;
    private final List<Movimentacao> movimentacoes;

    public ContaCorrente(int numero, double saldoInicial) {
        validarValor("Saldo inicial", saldoInicial);
        this.especial = false;
        this.limite = 0;
        this.numero = numero;
        this.saldo = saldoInicial;
        this.movimentacoes = new ArrayList<Movimentacao>();
        this.criarMovimentacao("Saldo inicial", 'C', saldoInicial);
    }

    public ContaCorrente(int numero, double saldoInicial, double limite) {
        validarValor("Saldo inicial", saldoInicial);
        validarValor("Limite", limite);
        this.especial = true;
        this.limite = limite;
        this.numero = numero;
        this.saldo = saldoInicial;
        this.movimentacoes = new ArrayList<Movimentacao>();
        this.criarMovimentacao("Saldo inicial", 'C', saldoInicial);
    }

    public int getNumeroConta() {
        return this.numero;
    }

    protected void depositar(double valor) {
        validarValor("Deposito", valor);
        this.saldo += valor;
        this.criarMovimentacao("Deposito", 'C', valor);
    }

    protected void sacar(double valor) {
        validarValor("Saque", valor);
        if (this.saldo + this.limite < valor) {
            throw new BancoException("Saldo insuficiente para saque.");
        }
        this.saldo -= valor;
        this.criarMovimentacao("Saque", 'D', valor);
    }

    private void criarMovimentacao(String descricao, char tipo, double valor) {
        this.movimentacoes.add(new Movimentacao(descricao, tipo, valor));
    }

    private void validarValor(String rotulo, double valor) {
        if (Double.isNaN(valor) || Double.isInfinite(valor) || valor < 0) {
            throw new BancoException(rotulo + " deve ser um valor valido e nao negativo.");
        }
    }

    protected String emitirExtrato() {
        StringBuilder extrato = new StringBuilder("Extrato bancario C/C " + numero);
        for (Movimentacao movimentacao : movimentacoes) {
            extrato.append("\n").append(movimentacao.getMovimentacao());
        }
        extrato.append("\n Saldo final R$ ").append(this.saldo);
        if (especial) {
            extrato.append("\n Limite R$ ").append(this.limite);
        }
        return extrato.toString();
    }
}
