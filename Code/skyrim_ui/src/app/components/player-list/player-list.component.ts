import { Component, EventEmitter, OnDestroy, OnInit, Output, ViewEncapsulation } from "@angular/core";
import { PlayerList } from "src/app/models/player-list";
import { ClientService } from "src/app/services/client.service";
import { PlayerListService } from "src/app/services/player-list.service";

@Component({
    selector: 'app-player-list',
    templateUrl: './player-list.component.html',
    styleUrls: ['./player-list.component.scss'],
    encapsulation: ViewEncapsulation.None
})
export class PlayerListComponent implements OnInit, OnDestroy {

    @Output()
    public done = new EventEmitter();

    constructor(public playerListService: PlayerListService,
        private clientService: ClientService
    ) {}

    ngOnDestroy(): void {

    }
    ngOnInit(): void {
    }

    public get playerList(): PlayerList | undefined {
        return this.playerListService.playerList.value;
    }

    public get isConnected(): boolean {
        return this.clientService.connectionStateChange.value;
    }

    public teleportToPlayer(playerId: number) {
        this.clientService.teleportToPlayer(playerId);
    }

    public cancel(): void {
        this.done.next();
    }
}