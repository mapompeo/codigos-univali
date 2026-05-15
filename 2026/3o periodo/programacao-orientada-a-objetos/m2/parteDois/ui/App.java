package parteDois.ui;

import java.util.Scanner;

import parteDois.dominio.Barco;
import parteDois.dominio.Caminhao;
import parteDois.dominio.Carro;
import parteDois.dominio.Galpao;
import parteDois.dominio.Helicoptero;

public class App {
    private final Scanner scanner = new Scanner(System.in);
    private final Galpao galpao = new Galpao();

    public void executar() {
        int opcao;
        do {
            opcao = lerOpcaoMenu();
            switch (opcao) {
                case 1 -> adicionarBarco();
                case 2 -> adicionarHelicoptero();
                case 3 -> adicionarCarro();
                case 4 -> adicionarCaminhao();
                case 5 -> galpao.imprimirRelacaoVeiculos();
                case 0 -> System.out.println("Saindo...");
                default -> System.out.println("Opção inválida.");
            }
        } while (opcao != 0);
    }

    private int lerOpcaoMenu() {
        System.out.println();
        System.out.println("=== Galpão do Bruce ===");
        System.out.println("1 - Adicionar Barco");
        System.out.println("2 - Adicionar Helicóptero");
        System.out.println("3 - Adicionar Carro");
        System.out.println("4 - Adicionar Caminhão");
        System.out.println("5 - Imprimir relação de veículos");
        System.out.println("0 - Sair");
        System.out.print("Opção: ");

        String input = scanner.nextLine().trim();
        try {
            return Integer.parseInt(input);
        } catch (NumberFormatException e) {
            return -1;
        }
    }

    private void adicionarBarco() {
        System.out.print("Nome do barco: ");
        String nome = scanner.nextLine().trim();

        Barco barco = new Barco(nome);

        System.out.print("Nome do capitão: ");
        String capitao = scanner.nextLine().trim();
        barco.informarCapitao(capitao);

        galpao.adicionarVeiculo(barco);
        System.out.println("Barco adicionado.");
    }

    private void adicionarHelicoptero() {
        System.out.print("Modelo do helicóptero: ");
        String modelo = scanner.nextLine().trim();

        System.out.print("Sigla: ");
        String sigla = scanner.nextLine().trim();

        Helicoptero helicoptero = new Helicoptero(modelo, sigla);

        System.out.print("Nome do piloto: ");
        String piloto = scanner.nextLine().trim();
        helicoptero.atribuirPiloto(piloto);

        galpao.adicionarVeiculo(helicoptero);
        System.out.println("Helicóptero adicionado.");
    }

    private void adicionarCarro() {
        System.out.print("Modelo do carro: ");
        String modelo = scanner.nextLine().trim();

        System.out.print("Placa: ");
        String placa = scanner.nextLine().trim();

        double potencia = lerDouble("Potência: ");

        Carro carro = new Carro(modelo, placa, potencia);

        System.out.print("Nome do motorista: ");
        String motorista = scanner.nextLine().trim();
        carro.atribuirMotorista(motorista);

        galpao.adicionarVeiculo(carro);
        System.out.println("Carro adicionado.");
    }

    private void adicionarCaminhao() {
        System.out.print("Modelo do caminhão: ");
        String modelo = scanner.nextLine().trim();

        System.out.print("Placa: ");
        String placa = scanner.nextLine().trim();

        double capacidadeCarga = lerDouble("Capacidade de carga: ");

        Caminhao caminhao = new Caminhao(modelo, placa, capacidadeCarga);

        System.out.print("Nome do motorista: ");
        String motorista = scanner.nextLine().trim();
        caminhao.atribuirMotorista(motorista);

        galpao.adicionarVeiculo(caminhao);
        System.out.println("Caminhão adicionado.");
    }

    private double lerDouble(String prompt) {
        while (true) {
            System.out.print(prompt);
            String input = scanner.nextLine().trim();
            try {
                return Double.parseDouble(input.replace(',', '.'));
            } catch (NumberFormatException e) {
                System.out.println("Valor inválido. Tente novamente.");
            }
        }
    }
}
