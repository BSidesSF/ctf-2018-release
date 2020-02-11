var WIDTH = 80;
var HEIGHT = 50;

function makeCrossword(crosswordUrl, wordlistUrl){
  var table = $('<table></table>');
  table.attr('id','grid');
  $('#test-zone').prop('count', 0);
  $('#test-zone').prop('max', 10);

  $("#grid-container").append(table);

  $.getJSON(crosswordUrl, function(data){
  }).done(function(response){
    $("#grid-container").css('min-width', response["width"] * WIDTH + 'px');
    $("#grid-container").css('min-height', response["height"] * HEIGHT + 'px');
    for (var i = 0; i < response["height"]; i++){
      var row = $("<tr></tr>");
      table.append(row);
      for (var j=0; j < response["width"]; j++){
        row.append($("<td class='dropcell square'></td>"));
      }
    }

    answerFields = response["fields"]
    var idx = 0;
    response["fields"].forEach(function(field) {
      var p = $("<p class='answer'></p>");
      p.attr("id", idx);
      idx += 1
      //p.css("border", "4px solid " + field["color"]);
      p.css("background-color", field["color"]);
      p.addClass(field["direction"]);
      if (field["direction"] == "down")
        $("#down").append(p);
      else
        $("#across").append(p);

      if (field["direction"] == "down")
      var rd = 1, cd = 0;
      else
      var rd = 0, cd = 1;
      for (var i=0, r=field["row"], c=field["col"]; i < field["length"]; i+=1,r+=rd,c+=cd){
        var cell = getCell(r, c);
        if (i == 0)
          cell.css('background-color', field["color"]);
        cell.addClass('active');
      }
    });
    // create the wordlist
    $.each(response["wordlist"], function(key, count){
      for (var i = 0; i < count; i++){
        $('#wordlist').append("<div class='dropcell placeholder occupied'>\
        <div class='square token draggable'>" + key + "</div></div>");
      }
    });
    // set up the drag n drop
    $('.draggable').draggable({
      start: dragStart,
      revert: 'invalid',
      snap: true,
      snapMode: "inner",
    });
    $('.dropcell').droppable({
      drop: dropCell,
      tolerance: 'pointer',
    });
    $('.dropzone').droppable({
      drop: dropZone,
      tolerance: 'pointer',
    })
  });
}

function getCell(row, col){
  return $('#grid tr:eq(' + row + ') td:eq(' + col + ')');
}

function dragStart(event, ui){
  $(this).css('z-index', 1337);
}
function dropCell(event, ui){
  if ($(this).hasClass('occupied')) {
    ui.draggable.draggable('option', 'revert', true);
    return false;
  }
  $(this).append(ui.draggable);
  ui.draggable.position({of: $(this), my: 'left top', at: 'left top'});
  ui.draggable.css('z-index', 0);
  ui.draggable.css('left', '0px');
  ui.draggable.css('top', '0px');
  if ($(this).hasClass('active')) {
    ui.draggable.css('background-color', $(this).css('background-color')); 
  } else {
    ui.draggable.css('background-color', '#a5acaa');
  }
  validate();
}

function dropZone(event, ui){
  var count = $(this).prop('count');
  if (count >= $(this).prop('max'))
    return

  $(this).prop('count', count + 1);
  $(this).append(ui.draggable);
  ui.draggable.position({of: $(this), my: 'left top', at: 'left top'});
  ui.draggable.css('z-index', 0);
  ui.draggable.css('left', count * WIDTH + 'px');
  ui.draggable.css('top', '0px');
}

function validate() {
  $(".dropcell").each(function(){
    if ($(".draggable", this)[0]) {
      $(this).addClass("occupied");
    } else {
      $(this).removeClass("occupied");
    }
  });
  checkAnswers();
}

function checkAnswers(){
  var answers = {};
  for (var i = 0; i < answerFields.length; i++){
    var solution = "";
    for (var j=0, r=answerFields[i]["row"], c=answerFields[i]["col"];
      j < answerFields[i]["length"]; j++,
      r += answerFields[i]["direction"] == "down" ? 1: 0,
      c += answerFields[i]["direction"] == "down" ? 0: 1
    ) {
      var cell = getCell(r, c);
      if (cell.hasClass('occupied')){
        if (!(r in answers))
          answers[r] = {};
        char = cell.find('.token').text();
        answers[r][c] = char;
        solution += char + " ";
      }
    }
    $("#" + i).text(solution);
  }
  $.post("/check", {"answers": JSON.stringify(answers)}, function(data){
    if (data['Status'] == 'flag'){
        alert('Congrats! Here is your flag: ' +  data['Message']);
        return;
    }

    if (data["Status"] != "ok"){
        alert("No cheating!");
        return;
    }

    data = data['Results'];
    for (var i = 0; i < data.length; i++){
      if (data[i] == "correct"){
        $('#' + i).addClass('correct')
        $('#' + i).removeClass('incorrect')
      } else if (data[i] == "incorrect" || data[i] == "syntax error") {
        $('#' + i).removeClass('correct')
        $('#' + i).addClass('incorrect')
      }
      else {
        $('#' + i).removeClass('correct')
        $('#' + i).removeClass('incorrect')
      }
    }
  });
}
