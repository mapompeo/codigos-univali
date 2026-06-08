using System;

class Paciente
{
    // atributos privados
    private string nome;
    private double peso;
    private double altura;

    // inicializa os atributos e valida os dados
    public Paciente(string nome, double peso, double altura)
    {
        if (peso <= 0 || altura <= 0)
            throw new ArgumentException("Peso e altura devem ser valores positivos.");

        this.nome = nome;
        this.peso = peso;
        this.altura = altura;
    }

    public double CalcularIMC()
    {
        return peso / (altura * altura);
    }

    // classificação com base na tabela da oms
    public string Classificar()
    {
        double imc = CalcularIMC();

        if (imc < 18.5) return "Abaixo do peso";
        if (imc < 25.0) return "Peso normal";
        if (imc < 30.0) return "Sobrepeso";
        if (imc < 35.0) return "Obesidade Grau I";
        if (imc < 40.0) return "Obesidade Grau II";
        return "Obesidade Grau III";
    }

    // exibe o resultado completo
    public void Exibir()
    {
        double imc = CalcularIMC();

        Console.WriteLine("\n========== RESULTADO ==========");
        Console.WriteLine($"Paciente:       {nome}");
        Console.WriteLine($"Peso:           {peso:F2} kg");
        Console.WriteLine($"Altura:         {altura:F2} m");
        Console.WriteLine($"IMC:            {imc:F2}");
        Console.WriteLine($"Classificação:  {Classificar()}");
        Console.WriteLine("================================");
    }
}

class Program
{
    static void Main()
    {
        // coleta os dados e cria o objeto Paciente
        Console.WriteLine("=== Calculadora de IMC (Orientada a Objetos - C#) ===\n");

        Console.Write("Digite o nome do paciente: ");
        string nome = Console.ReadLine();

        Console.Write("Digite o peso (kg): ");
        double peso = double.Parse(Console.ReadLine());

        Console.Write("Digite a altura (m): ");
        double altura = double.Parse(Console.ReadLine());

        try
        {
            Paciente paciente = new Paciente(nome, peso, altura);
            paciente.Exibir();
        }
        catch (ArgumentException e)
        {
            Console.WriteLine($"Erro: {e.Message}");
        }
    }
}
