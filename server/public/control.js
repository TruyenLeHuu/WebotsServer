var socket = io.connect('http://127.0.0.1:3001', { transports : ['websocket'] });
$(document).ready(function () {
    const form = document.querySelector('#formGroup')
    const name = document.querySelector('#name')
    const index = document.querySelector('#index')
    const idTeamA = document.querySelector('#idTeamA')
    const idTeamB = document.querySelector('#idTeamB')

    const updtForm = document.querySelector('#updtForm')
    const updateName = document.querySelector('#updateName')
    const updateIndex = document.querySelector('#updateIndex')
    const updateIdTeamA = document.querySelector('#updateIdTeamA')
    const updateIdTeamB = document.querySelector('#updateIdTeamB')

    const delForm = document.querySelector('#delForm')
    const delName = document.querySelector('#delName')
    
    const formTeam = document.querySelector('form')
    const idTeam = document.querySelector('#id')
    const nameTeam = document.querySelector('#nameTeam')
    const controller = document.querySelector('#controller')
    const school = document.querySelector('#school')

    const updtFormTeam = document.querySelector('#updtFormTeam')
    const updateId = document.querySelector('#updateId')
    const updateNameTeam = document.querySelector('#updateNameTeam')
    const updateController = document.querySelector('#updateController')
    const updateSchool = document.querySelector('#updateSchool')

    const delFormTeam = document.querySelector('#delFormTeam')
    const delIdTeam = document.querySelector('#delIdTeam')
    const table = document.getElementById('example');

    var currentData;

    if(socket !== undefined){
      console.log('Connected to socket...');
      // Handle Output
      // var elem;
      // var buffer;
      // var idd;
      // var ImgCell; 
      // var replacedTime;
      // socket.emit('requestChangeData')
      // socket.on('changeData', async function (data){
      //   console.info(data)
      //   currentData = data;
      //   const tbody = table.getElementsByTagName('tbody')[0];
      //   tbody.innerHTML = '';
      //   // $('#studentsList').empty()
      //   if(data.length){       
      //     for(var x = 0; x < data.length; x++)
      //     {
      //       if(data[x].time.length > 0) 
      //         replacedTime = data[x].time[0].replace(/\s/g, "_");
      //       await $('#studentsList').append(` <tr>
      //                                         <th scope="row" class="sorting_1">${(x+1)}</th>
      //                                         <td>${data[x].id}</td>
      //                                         <td>${data[x].name}</td>
      //                                         <td>${data[x].mssv}</td>
      //                                         <td>${data[x].subject}</td>
      //                                         <td>${data[x].teacher}</td>
      //                                         <td>${data[x].time.length > 0 ? data[x].time[0] : ""}</td>
      //                                         <td class = "t${data[x].time.length > 0 ? replacedTime : ""}"></td>
      //                                         </tr>`);
      //       for (var i = 1; i < data[x].time.length; i++)
      //       {
      //         replacedTime = data[x].time[i].replace(/\s/g, "_");
      //         await $('#studentsList').append(` <tr>
      //                                           <th scope="row" class="sorting_1"></th>
      //                                           <td></td>
      //                                           <td></td>
      //                                           <td></td>
      //                                           <td></td>
      //                                           <td></td>
      //                                           <td>${data[x].time[i]}</td>
      //                                           <td class = "t${replacedTime}"></td>
      //                                           </tr>`);
      //       }
      //       if(data[x].time.length && data[x].time[0] != '' )
      //       {
      //         for (var i = 0; i < data[x].time.length; i++)
      //         {
      //           replacedTime = data[x].time[i].replace(/\s/g, "_");
      //           await socket.emit('requestImg', replacedTime);
      //           // ImgCell = await document.querySelector(`.t${data[x].time[i]}`);
      //           // ImgCell.innerHTML = `<img src="images/${data[x].time[i]}">`;
      //           // const imgTag = document.createElement("img");
      //           // imgTag.src = "images/"+replacedTime;
      //           // ImgCell.appendChild(imgTag);
      //         }
      //       }
      //     } 
      //           } else { $('#cmn').append(`<tr>
      //               <td colspan="12">${'There no one at all'}</td>
      //               </tr>`)}
      // });
      // socket.on('image-data', imageData => {
      //   if (imageData.image) {
      //     // replacedTime = imageData.time.replace(/\s/g, "_");
      //     ImgCell = document.querySelector(`.t${imageData.time}`);
      //     ImgCell.innerHTML = `<img src="${imageData.image}">`;
      //     // const imgTag = document.createElement("img");
      //     // imgTag.src = imageData.image;
      //     // ImgCell.appendChild(imgTag);
      //   }
      // });
      // socket.on('updateImage', (data)=>{
      //   for(var y = 0;y < data.length;y++){
      //   if (data[y].image) {
      //     // console.log(data[y].image);
      //     idd = "img"+y
      //     console.log(idd)
      //     elem = document.createElement("img");
      //     buffer = Buffer.from(data[y].image, "base64");  
      //     Jimp.read(buffer,  (err, res) => {
      //         res.getBase64Async(jimp.MIME_PNG).then((newImage) => {
      //         elem.src = newImage;  
      //         document.getElementById(idd.toString()).appendChild(elem);
      //       })
      //     })
      //   }
      // }})
      // socket.on('changeTemHum', (data)=>{
      //   $('#TempHum').empty()
      //   $('#TempHum').append(`<div>🌡 Temp: ${data.temp}°C 💧 Hum: ${data.humidity}%</div>`)
      // })
  }
  formTeam.addEventListener('submit', async (e) => {
    console.log("addTeam")
       e.preventDefault()     
       try {
            const res = await fetch('/addTeam', {
              method: 'POST',
              body: JSON.stringify({ id: idTeam.value , name: nameTeam.value, controller: controller.value, school: school.value}),
              headers: { 'Content-Type': 'application/json' }
              })  
          } catch (err) {
            console.log(err.message)
          }
        })
      delFormTeam.addEventListener('submit', async (e) => {
        console.log("deleteTeam")
        e.preventDefault()     
        try { 
              const res = await fetch('/deleteTeam', {
              method: 'POST',
              body: JSON.stringify({id: delIdTeam.value}),
              headers: { 'Content-Type': 'application/json' }
              }) 

              } catch (err) {
                console.log(err.message)
              }
             })
    updtFormTeam.addEventListener('submit', async (e) => {
      e.preventDefault()     
      console.log("updateTeam")
      try {
            const res = await fetch('/updateTeam', {
            method: 'POST',
            body: JSON.stringify({ id: updateId.value , name: updateNameTeam.value, controller: updateController.value, school: updateSchool.value}),
            headers: { 'Content-Type': 'application/json' }
            })   
         } catch (err) {
           console.log(err.message)
         }
       })

      form.addEventListener('submit', async (e) => {
      e.preventDefault()     
      try {
        console.log("addGroup")
            const res = await fetch('/addGroup', {
              method: 'POST',
              body: JSON.stringify({ name: name.value , index: index.value, idTeamA: idTeamA.value, idTeamB: idTeamB.value}),
              headers: { 'Content-Type': 'application/json' }
              })  
          } catch (err) {
            console.log(err.message)
          }
        })
     delForm.addEventListener('submit', async (e) => {
         e.preventDefault()     
         try { 
          console.log("deleteGroup")
               const res = await fetch('/deleteGroup', {
               method: 'POST',
               body: JSON.stringify({name: delName.value}),
               headers: { 'Content-Type': 'application/json' }
               }) 
 
               } catch (err) {
                 console.log(err.message)
               }
              })
     updtForm.addEventListener('submit', async (e) => {
       e.preventDefault()     
       try {
        console.log("updateGroup")
             const res = await fetch('/updateGroup', {
             method: 'POST',
             body: JSON.stringify({ name: updateName.value , index: updateIndex.value, idTeamA: updateIdTeamA.value, idTeamB: updateIdTeamB.value}),
             headers: { 'Content-Type': 'application/json' }
             })   
          } catch (err) {
            console.log(err.message)
          }
        })
             
    var acc = document.getElementsByClassName("accordion");
    var i;
    for (i = 0; i < acc.length; i++) {
    acc[i].nextElementSibling.style.display = "none"
    }
    for (i = 0; i < acc.length; i++) {
      acc[i].addEventListener("click", function() {
        /* Toggle between adding and removing the "active" class,
        to highlight the button that controls the panel */
        this.classList.toggle("active");

        /* Toggle between hiding and showing the active panel */
        var panel = this.nextElementSibling;
        if (panel.style.display === "block") {
          panel.style.display = "none";
        } else {
          panel.style.display = "block";
        }

      });
    }
})