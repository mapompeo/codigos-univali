package br.univali.cc.prog3.banco.visao;

import br.univali.cc.prog3.banco.dominio.Banco;
import br.univali.cc.prog3.banco.dominio.BancoException;
import java.util.Scanner;

public class BancoGUI {
    private Banco banco;
    private Scanner leitor = new Scanner(System.in);

    public BancoGUI(Banco banco) {
        this.banco = banco;
    }

    public String lerValor(String rotulo) {
        System.out.print(rotulo + ": ");
        return leitor.nextLine();
    }

    public void menu() {
        char opcao;
        do {
            System.out.println("Menu do " + this.banco.getNome());
            System.out.println("1 - Criar conta simples");
            System.out.println("2 - Criar conta especial");
            System.out.println("3 - Depositar");
            System.out.println("4 - Sacar");
            System.out.println("5 - Transferir");
            System.out.println("6 - Extrato");
            System.out.println("S - Sair");
            opcao = lerOpcao();
            try {
                switch (opcao) {
                    case '1': criarContaSimples(); break;
                    case '2': criarContaEspecial(); break;
                    case '3': depositar(); break;
                    case '4': sacar(); break;
                    case '5': transferir(); break;
                    case '6': extrato(); break;
                    case 'S': break;
                    default: System.out.println("Opcao invalida.");
                }
            } catch (BancoException ex) {
                System.out.println("Erro: " + ex.getMessage());
            }
        } while (opcao != 'S');
    }

    private char lerOpcao() {
        String texto = this.lerValor("Selecione uma opcao").trim().toUpperCase();
        if (texto.length() == 0) {
            return ' ';
        }
        return texto.charAt(0);
    }

    private int lerInteiro(String rotulo) {
        while (true) {
            try {
                return Integer.parseInt(lerValor(rotulo).trim());
            } catch (NumberFormatException ex) {
                System.out.println("Informe um numero inteiro valido.");
            }
        }
    }

    private double lerValorMonetario(String rotulo) {
        while (true) {
            try {
                String texto = lerValor(rotulo).trim().replace(',', '.');
                double valor = Double.parseDouble(texto);
                if (Double.isNaN(valor) || Double.isInfinite(valor) || valor < 0) {
                    System.out.println("Informe um valor valido e nao negativo.");
                } else {
                    return valor;
                }
            } catch (NumberFormatException ex) {
                System.out.println("Informe um valor numerico valido.");
            }
        }
    }

    private void criarContaSimples() {
        int numeroConta = lerInteiro("Informe o numero da conta");
        double saldoInicial = lerValorMonetario("Informe o saldo inicial");
        this.banco.criarConta(numeroConta, saldoInicial);
    }

    private void criarContaEspecial() {
        int numeroConta = lerInteiro("Informe o numero da conta");
        double saldoInicial = lerValorMonetario("Informe o saldo inicial");
        double limite = lerValorMonetario("Informe o limite da conta");
        this.banco.criarConta(numeroConta, saldoInicial, limite);
    }

    private void depositar() {
        int numero = lerInteiro("Informe o numero da conta");
        double valor = lerValorMonetario("Informe o valor para deposito");
        this.banco.depositar(numero, valor);
    }

    private void sacar() {
        int numero = lerInteiro("Informe o numero da conta");
        double valor = lerValorMonetario("Informe o valor para saque");
        this.banco.sacar(numero, valor);
    }

    private void transferir() {
        int numeroOrigem = lerInteiro("Informe o numero da conta de origem");
        int numeroDestino = lerInteiro("Informe o numero da conta de destino");
        double valor = lerValorMonetario("Informe o valor para transferencia");
        this.banco.transferir(numeroOrigem, numeroDestino, valor);
    }

    private void extrato() {
        int numero = lerInteiro("Informe o numero da conta");
        System.out.println(this.banco.emitirExtrato(numero));
    }
}
