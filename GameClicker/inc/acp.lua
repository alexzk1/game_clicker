require "helper"
---------------------------���������----------------------------------------
--���������� ������ �� ������� �� � ���������� �������� ������� ����

acpX = 142
acpY = 28
acpC = 0x00206485

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
 end
end


math.randomseed(os.time())
registerHotKey("ctrl+F11","acp","��������/��������� ACP")
