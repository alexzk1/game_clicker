require "helper1"
require "colors"

---------------------------���������----------------------------------------
--���������� ������ �� ������� �� � ���������� �������� ������� ����

acpX = 147
acpY = 29
acpC = rgb(136,90,0)

----------------------------------------------------------------------------



function randDelay()
 --��������� �������� (����� ����� + ������� �������)
 delay(200 + math.random(150,600))
 --������� ��������� �������, ��� �� ��������
end

function acp()
 while (getPressCount("acp") % 2) == 1 do
   if isActiveWin(1) then

      if getApiVersion()>100 then
         textOut(1,24,81,"ACP ��������")
      end

     if math.random()<0.85 then  
     --���� ������������ 85% ���� ������� ��������� �� ���� �������

        if getColor(1,acpX,acpY) ~= acpC then
           pressKx(1,3) --��� 3 ������ 3
        end
     end --end of random
   end
   randDelay()
   --delay(1000)
 end
end


math.randomseed(os.time())
registerHotKey("ctrl+F10","acp","��������/��������� ACP")

