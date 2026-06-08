package parteDois.dominio;

public abstract class VeiculoTerrestre implements Veiculo {
    protected String modelo;
    protected String placa;
    protected String motorista;

    public VeiculoTerrestre(String modelo, String placa) {
        this.modelo = modelo;
        this.placa = placa;
    }

    public void atribuirMotorista(String nome) {
        this.motorista = nome;
    }

    public String getPlaca() {
        return placa;
    }

    public String detalhar() {
        return "Modelo: " + modelo + ", Placa: " + placa + ", Motorista: " + motorista;
    }

    @Override
    public String obterDescricao() {
        return detalhar();
    }
}