# Custom Mesaging Protocol

## Client Messaging
Client messages are text messages sent from the client to the server. I decided to use text to keep things simple; however, a nice improvement would be to switch to a binary protocol
to decrease the number of bytes transmitted between client and server. This is a tradeoff of CPU power on the server side and size of the messages sent through the network.

#### Join Lobby Client Message 0{roomId}
- Begins with 0 and is followed by an 8 character long room id
- e.g. `0jTBj32Ij`

#### Start Game Client Message 1
- Begins with 1 with no following characters
- e.g. `1`

#### Move Client Message 2...
- Begins with 2 and the following characters depends on the move requested

##### Play Move 0{playMove}{merchantCardId}...
- Begins with 0 followed by 2 characters indicating the merchant card id
- The following characters depends on the play move requested

###### Crystal Play Move 0{merchantCardId}
- Code is 0
- e.g. `20000`

###### Upgrade Play Move 1{merchantCardId}{numUpgrades}{upgrades}
- Code is 1
- Number of upgrades is 1 character long
- Upgrades is `2 * numUpgrades` characters long
    - Each upgrade is 2 characters long where the first character is the crystal to be upgraded [0, 3] and the second character is the crystal to upgrade to [1, 4]
- e.g. `2010120112`

###### Trade Play Move 2{merchantCardId}{numTrades}
- Code is 2
- Number of trades is 2 characters long [1, 10]
- e.g. `2021401`

##### Acquire Move 1{merchantCardId}
- Begins with 1 followed by 2 characters indicating the merchant card id
- e.g. `2100`

##### Rest Move 2
- Begins with 2 with no following characters
- e.g. `22`

##### Claim Move 3{pointCardId}
- Begins with 3 followed by 2 characters indicating the point card id
- e.g. `2300`

#### Remove Crystal Overflow Client Message 3{crystals}
- Begins with 3 followed by crystals
- Crystals is 8 characters long where each 2 successive characters indicates yellow, green, blue, pink number of crystals [0, 10]

#### Chat Client Message 4{text}
- text is dynamically sized string of ASCII characters
- e.g. `4hello world`

#### Change Name Client Message 5{name}
- name is dynamically sized string of ASCII characters
- e.g. `5newName`

## Server Messaging
Server messages are text messages sent from server to the client. I decided to use JSON to structure my messages. Each message always has a `messageType` and `payload` property.

#### User ID Server Message 0
- `messageType` property has value 0
- `payload` property contains object with key value pair `userId: value`

#### Lobby Server Message 1
- `messageType` property has value 1
- `payload` property is a serialized `Lobby` object

#### Game Server Message 2
- `messageType` property has value 2
- `payload` property is a serialized `Game` object
