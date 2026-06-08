package parteUm.dominio;

public class Carro extends VeiculoTerrestre {
    private double potencia;
    
    public Carro(String modelo, String placa, double potencia) {
        super(modelo, placa);
        this.potencia = potencia;
    }

    public double getPotencia() {
        return potencia;
    }
} 