package parteDois.dominio;

public class Carro extends VeiculoTerrestre {
    private double potencia;
    
    public Carro(String modelo, String placa, double potencia) {
        super(modelo, placa);
        this.potencia = potencia;
    }

    public double getPotencia() {
        return potencia;
    }

    @Override
    public String obterDescricao() {
        return super.detalhar() + ", Potência: " + potencia;
    }
} 