var socket = io.connect('http://127.0.0.1:3001', { transports : ['websocket'] });

$(document).ready(function () {
  const table = document.getElementById('results-table');
  if(socket !== undefined){
    console.log('Connected to socket...');
    socket.on('changeResults', async function (data){
      console.info(data)
      // const tbody = table.getElementsByTagName('tbody')[0];
      // tbody.innerHTML = '';
      $('#results-list').empty()
      if(data.length){       
        for(var x = 0; x < data.length; x++)
        {
          if (x > 2) {
            if (x < 16)
            { await $('#results-list').append(` <tr class="pos">
                                            <td scope="row">${(x+1)}</td>
                                            <td>${data[x].name}</td>
                                            <td>${data[x].school}</td>
                                            <td>${data[x].score}</td>
                                            <td>${data[x].time}</td>
                                            </tr>`); }
            else 
            {
              await $('#results-list').append(` <tr>
                                            <td scope="row">${(x+1)}</td>
                                            <td>${data[x].name}</td>
                                            <td>${data[x].school}</td>
                                            <td>${data[x].score}</td>
                                            <td>${data[x].time}</td>
                                            </tr>`);
            }
          }
          else 
          {  
            await $('#results-list').append(` <tr class="wpos">
                                          <td scope="row">${(x+1)}</td>
                                          <td>${data[x].name}</td>
                                          <td>${data[x].school}</td>
                                          <td>${data[x].score}</td>
                                          <td>${data[x].time}</td>
                                          </tr>`);
          }
        }
      }
    });
  }
})
document.addEventListener("DOMContentLoaded", function() {
  const images = ['./1964_Shelby_Cobra_289_MkII_supercar_supercars_classic_muscle_e_2048x1536.jpg', './1964_Shelby_Cobra_USRRC_Roadster_CSX_2557_race_racing_supercar_supercars_classic_muscle___r_2048x1536.jpg', './2006_Caparo_T1_supercar_supercars_race_racing_____g_2048x1536.jpg']; // Thêm đường dẫn thực tế đến các hình ảnh của bạn vào đây
  const background = document.getElementById('background');

  function changeBackground() {
      const randomIndex = Math.floor(Math.random() * images.length);
      background.style.backgroundImage = `url('${images[randomIndex]}')`;
  }

  changeBackground(); // Để hiển thị hình nền ban đầu ngay khi trang web được tải
  setInterval(changeBackground, 10000); // Đặt thời gian cách nhau là 10000 ms (10 giây)
});