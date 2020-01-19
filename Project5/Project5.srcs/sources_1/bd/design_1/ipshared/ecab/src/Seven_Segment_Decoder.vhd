library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity Seven_Segment_Decoder is
    Port ( inn : in STD_LOGIC_VECTOR (3 downto 0);
           uit : out STD_LOGIC_VECTOR (7 downto 0));
end Seven_Segment_Decoder;

architecture Behavioral of Seven_Segment_Decoder is

begin
process(inn)
begin
    case inn is
        when "0000" => uit <= "11111100";
        when "0001" => uit <= "10010000";
        when "0010" => uit <= "11100110";
        when "0011" => uit <= "11110010";
        when "0100" => uit <= "10011010";
        when "0101" => uit <= "01111010";
        when "0110" => uit <= "01111110";
        when "0111" => uit <= "11010000";
        when "1000" => uit <= "11111110";
        when "1001" => uit <= "11111010";
        when "1010" => uit <= "11011111";
        when "1011" => uit <= "11111111";
        when "1100" => uit <= "01101101";
        when "1110" => uit <= "11111101";
        when "1111" => uit <= "01001111";
        when others => uit <= "00000000";
    end case;
end process;

end Behavioral;
