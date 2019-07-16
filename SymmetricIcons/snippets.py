def generatecolors(self):
    def genrainbow(x, y):
        return [self.makecolor([0, y, 255]), self.makecolor([0, 255, 255 - y]),
                self.makecolor([y, 255, 0]), self.makecolor([255, 255 - y, 0]),
                self.makecolor([255, 0, y]), self.makecolor([255 - y, 0, 255])][x % 6]

    aofcolor = [0] * 2112
    for i in range(64):
        aofcolor[i] = self.makecolor([0, 0, 4 * i])
    for i in range(256):
        localColor = self.makecolor([255, i, 255])
        aofcolor[(1344 + 3 * i)] = localColor
        aofcolor[(1345 + 3 * i)] = localColor
        aofcolor[(1346 + 3 * i)] = localColor

    for j in range(5):
        for i in range(256):
            aofcolor[64 + j * 256 + i] = genrainbow(j, i);
    return aofcolor
