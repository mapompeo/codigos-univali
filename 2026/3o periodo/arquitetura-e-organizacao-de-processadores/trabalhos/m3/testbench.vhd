library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity testbench is
end testbench;

architecture qualquernome of testbench is
    signal w_a, w_b : std_logic_vector(31 downto 0);
    signal w_sel : std_logic_vector(3 downto 0);
    signal w_data : std_logic_vector(31 downto 0);
    signal w_zero : std_logic;

begin
    DUT : entity work.design
    port map (w_a, w_b, w_sel, w_data, w_zero);

    process
    begin
        w_a <= std_logic_vector(to_signed(10, 32));
        w_b <= std_logic_vector(to_signed(5, 32));

        w_sel <= "0000"; wait for 5 ns; -- ADD  : 10 + 5 = 15
        w_sel <= "0001"; wait for 5 ns; -- SUB  : 10 - 5 = 5
        w_sel <= "0010"; wait for 5 ns; -- AND  : 10 & 5 = 0
        w_sel <= "0011"; wait for 5 ns; -- OR   : 10 | 5 = 15
        w_sel <= "0100"; wait for 5 ns; -- XOR  : 10 xor 5 = 15
        w_sel <= "0101"; w_b <= std_logic_vector(to_unsigned(2, 32)); wait for 5 ns; -- SLL  : 10 << 2 = 40
        w_sel <= "0110"; wait for 5 ns; -- SRL  : 10 >> 2 = 2
        w_sel <= "0111"; w_a <= std_logic_vector(to_signed(-16, 32)); w_b <= std_logic_vector(to_unsigned(2, 32)); wait for 5 ns; -- SRA  : -16 >> 2 = -4
        w_sel <= "1000"; w_a <= std_logic_vector(to_signed(-5, 32)); w_b <= std_logic_vector(to_signed(3, 32)); wait for 5 ns; -- SLT  : -5 < 3 => 1
        w_sel <= "1001"; w_a <= std_logic_vector(to_unsigned(5, 32)); w_b <= std_logic_vector(to_unsigned(10, 32)); wait for 5 ns; -- SLTU : 5 < 10 => 1

        wait;
    end process;

end qualquernome;