import { Component, OnInit, ElementRef, Renderer2   } from "@angular/core";
import { MessageService } from './MessageService';
      
@Component({
    selector: 'app-my-component',
    template: ``
  })
  export class MyComponent implements OnInit {
    message: string;
  
    constructor(private messageService: MessageService, private elementRef: ElementRef, private renderer: Renderer2) { }


  
    ngOnInit() {
    let colors : string[] = ['red', 'green', 'blue', 'grey', 'maroon', 'magenta', 'orange', 'purple']

    function convertToRange(value: number): number {
      return (value % 8) + 1;
    }

    function colorPick (number: number) {
      return colors[number];
    }


      this.messageService.connect().subscribe((jsonString) => {
        // Разбираем JSON-строку в объект
        const data = JSON.parse(jsonString);
        console.log(data)

        data.events.forEach((event : any) => {
          const sectionElement = document.getElementById(event.section);
          if (sectionElement != null) {
            if (event.type == 1) {
              this.renderer.setStyle(sectionElement, 'fill', 'black')
            } else if (event.type == 2) {
              //this.renderer.setStyle(sectionElement, 'fill', '')
            } else if (event.type == 0) {
              this.renderer.setStyle(sectionElement, 'fill', colorPick(convertToRange(event.trainId)))
            } else {
              console.log('smth go wrong on fill section')
            }
          }
        });

  
        // Теперь вы можете использовать объект data для вашей логики
      });
    }
  }