### **📌 Copilot Instructions to Complete the Playlist Functionality**
Use this prompt in **Copilot Chat** inside VS Code to **complete the Playlist Component** with track selection and deck assignment.

---

### **Prompt:**
```plaintext
I need to complete the **playlist functionality** in my OtoDecks DJ application. The playlist should allow users to:

1. **Upload and Add Tracks** to the playlist.
2. **Select a Track and Assign it to a Deck** when pressing Play:
   - When a track is selected and Play is clicked, the user should be prompted to choose between **Deck 1 or Deck 2**.
   - The selected track should then be loaded into the corresponding `DeckGUI` as if it was uploaded directly.

---

### **Tasks to Implement**
1. **Modify `PlaylistComponent` to Allow Track Uploads**:
   - Add a **file selection dialog** to allow users to add MP3 files.
   - Store the **file paths** in a `std::vector<File>` list.
   - Display the track names in the playlist UI (`ListBox` or `TableListBox`).

2. **Enable Track Selection and Play Button Behavior**:
   - When the user selects a track and clicks Play:
     - Show a **popup dialog** or **console prompt** asking the user to choose Deck 1 or Deck 2.
     - Load the selected track into the corresponding `DeckGUI`.

3. **Modify `DeckGUI` to Accept Tracks from the Playlist**:
   - Update `DeckGUI::loadTrack()` to accept a file path passed from `PlaylistComponent`.
   - Ensure that `DJAudioPlayer::loadURL()` correctly loads the selected track.

4. **Ensure Smooth Integration**:
   - Make sure that the track starts playing immediately after loading into the selected deck.
   - Maintain track selection and highlight active tracks.

---

### **Expected Behavior**
- User **uploads MP3 files** → Files appear in the playlist.
- User **selects a track** and presses **Play** → A prompt appears for **Deck 1 or Deck 2**.
- The track **loads into the chosen deck** and plays immediately.

---

Now, generate the necessary code changes to implement this functionality in **PlaylistComponent** and **DeckGUI**.
```

---

### **💡 Next Steps**
1. **Paste this prompt into Copilot Chat inside VS Code**.
2. **Copilot will generate the code for PlaylistComponent and DeckGUI.**
3. **Copy and test the generated code** to ensure the feature works as expected.
4. **Let me know if anything needs refining! 🚀**