import { NgModule }      from "@angular/core";
import { BrowserModule } from '@angular/platform-browser';
import { MyComponent }   from './app.component';
import { MessageService } from './MessageService';
 
@NgModule({
    declarations: [ MyComponent ],
    imports:      [ BrowserModule ],

    bootstrap:    [ MyComponent ],
    providers:    [ MessageService ]
})
export class AppModule { }