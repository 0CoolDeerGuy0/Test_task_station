import { Component, OnInit  } from "@angular/core";
import { MessageService } from './MessageService';
      
@Component({
    selector: 'app-my-component',
    template: `<div>{{ message }}</div>`
  })
  export class MyComponent implements OnInit {
    message: string;
  
    constructor(private messageService: MessageService) { }
  
    ngOnInit() {
      console.log('im here!!')
      this.messageService.connect().subscribe((message) => {
        this.message = message.body;
        console.log(this.message)
      });
    }
  }