import { Injectable } from '@angular/core';
import { Client, Message } from '@stomp/stompjs';
import { Observable } from 'rxjs';

@Injectable({
  providedIn: 'root'
})
export class MessageService {
  private client: Client;

  constructor() { }

  connect(): Observable<any> {
    this.client = new Client({
      connectHeaders: {
        login: 'guest',
        passcode: 'guest',
      },
      brokerURL: 'ws://localhost:15674/ws',
      debug: function (str) {
        console.log(str);
      }
    });

    return new Observable<any>((observer) => {
      this.client.onConnect = (frame) => {
        this.client.subscribe('hellothere', (message) => {
          observer.next(message.body);
          console.log('received');
        });
      };

      this.client.onStompError = (frame) => {
        observer.error(frame.body);
      };

      this.client.activate();
    });
  }
}
