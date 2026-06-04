-- Code your testbench here
library IEEE;
use IEEE.std_logic_1164.all;

entity testbench is
end testbench;

architecture qualquernome of testbench is
	signal w_a, w_b : std_logic_vector(31 downto 0);
    signal w_sel : std_logic_vector(1 downto 0);
    signal w_data : std_logic_vector(31 downto 0);

begin
	DUT : entity work.design
    port map (w_a, w_b, w_sel, w_data);
    
    process
    begin
    	w_sel <= "00"; 
        w_a <= "00000000000000000000000000001010"; 
        w_b <= "00000000000000000000000000000101";  

        wait for 5 ns;
        
        
        w_sel <= "01"; 
        wait for 5 ns;
        
        w_sel <= "10"; 
        wait for 5 ns;
        
        w_sel <= "11";
        wait for 5 ns;


        
        wait;
    end process;

end qualquernome;