package br.univali.cc.banco;

import java.util.ArrayList;
import java.util.List;

public class Banco {
    private String nome;
    private int numero;
    private List<ContaCorrente> contas;

    public Banco(String nome, int numero) {
        this.nome = nome;
        this.numero = numero;
        this.contas = new ArrayList<>();
    }

    public void criarConta(double saldoInicial) {
        int num = gerarNumeroConta();
        contas.add(new ContaCorrente(num, saldoInicial));
        System.out.println("conta normal criada! número: " + num);
    }

    public void criarConta(double saldoInicial, double limite) {
        int num = gerarNumeroConta();
        contas.add(new ContaCorrente(num, saldoInicial, limite));
        System.out.println("conta especial criada! número: " + num + " | limite: R$ " + String.format("%.2f", limite));
    }

    private int gerarNumeroConta() {
        return contas.isEmpty() ? 1001 : contas.get(contas.size() - 1).getNumeroConta() + 1;
    }

    private ContaCorrente localizarConta(int numero) {
        for (ContaCorrente c : contas) {
            if (c.getNumeroConta() == numero) return c;
        }
        return null;
    }

    public void excluirConta(int numero) {
        ContaCorrente c = localizarConta(numero);
        if (c == null) { System.out.println("conta " + numero + " não encontrada."); return; }
        contas.remove(c);
        System.out.println("conta " + numero + " excluída.");
    }

    public void depositar(int numero, double valor) {
        ContaCorrente c = localizarConta(numero);
        if (c == null) { System.out.println("conta " + numero + " não encontrada."); return; }
        if (c.depositar(valor))
            System.out.println("depósito de R$ " + String.format("%.2f", valor) + " realizado. saldo: R$ " + String.format("%.2f", c.getSaldo()));
    }

    public void sacar(int numero, double valor) {
        ContaCorrente c = localizarConta(numero);
        if (c == null) { System.out.println("conta " + numero + " não encontrada."); return; }
        if (c.sacar(valor))
            System.out.println("saque de R$ " + String.format("%.2f", valor) + " realizado. saldo: R$ " + String.format("%.2f", c.getSaldo()));
    }

    public void emitirSaldo(int numero) {
        ContaCorrente c = localizarConta(numero);
        if (c == null) { System.out.println("conta " + numero + " não encontrada."); return; }
        System.out.println("saldo da conta " + numero + ": R$ " + String.format("%.2f", c.getSaldo()));
    }

    public String emitirExtrato(int numero) {
        ContaCorrente c = localizarConta(numero);
        if (c == null) return "conta " + numero + " não encontrada.";
        return c.emitirExtrato();
    }

    public void transferir(int contaOrigem, int contaDestino, double valor) {
        ContaCorrente origem = localizarConta(contaOrigem);
        ContaCorrente destino = localizarConta(contaDestino);
        if (origem == null) { System.out.println("conta de origem não encontrada."); return; }
        if (destino == null) { System.out.println("conta de destino não encontrada."); return; }
        if (origem.sacar(valor)) {
            destino.depositar(valor);
            System.out.println("transferência de R$ " + String.format("%.2f", valor) + " da conta " + contaOrigem + " para " + contaDestino + " realizada.");
        }
    }
}
