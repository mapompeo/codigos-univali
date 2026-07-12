package br.univali.cc.prog3.banco.dominio;

import java.util.HashMap;
import java.util.Map;

public class Banco {
    private final String nome;
    private final int numero;
    private final Map<Integer, ContaCorrente> contas;

    public Banco(String nome, int numero) {
        this.nome = nome;
        this.numero = numero;
        this.contas = new HashMap<Integer, ContaCorrente>();
    }

    public void criarConta(int numeroConta, double saldoInicial) {
        validarContaInexistente(numeroConta);
        this.contas.put(numeroConta, new ContaCorrente(numeroConta, saldoInicial));
    }

    public void criarConta(int numeroConta, double saldoInicial, double limite) {
        validarContaInexistente(numeroConta);
        this.contas.put(numeroConta, new ContaCorrente(numeroConta, saldoInicial, limite));
    }

    private ContaCorrente localizarConta(int numero) {
        ContaCorrente conta = this.contas.get(numero);
        if (conta == null) {
            throw new BancoException("Conta " + numero + " inexistente.");
        }
        return conta;
    }

    private void validarContaInexistente(int numeroConta) {
        if (this.contas.containsKey(numeroConta)) {
            throw new BancoException("Ja existe uma conta com o numero " + numeroConta + ".");
        }
    }

    public void depositar(int numero, double valor) {
        ContaCorrente contaCorrente = this.localizarConta(numero);
        contaCorrente.depositar(valor);
    }

    public void sacar(int numero, double valor) {
        ContaCorrente contaCorrente = this.localizarConta(numero);
        contaCorrente.sacar(valor);
    }

    public void transferir(int numeroOrigem, int numeroDestino, double valor) {
        ContaCorrente origem = this.localizarConta(numeroOrigem);
        ContaCorrente destino = this.localizarConta(numeroDestino);
        origem.sacar(valor);
        destino.depositar(valor);
    }

    public String emitirExtrato(int numero) {
        ContaCorrente contaCorrente = this.localizarConta(numero);
        return contaCorrente.emitirExtrato();
    }

    public String getNome() {
        return nome;
    }
}
