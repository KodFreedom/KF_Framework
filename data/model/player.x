xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 92;
 0.15000;0.93838;0.36200;,
 -0.15000;0.93838;0.36200;,
 -0.15000;0.88357;0.07499;,
 0.15000;0.88357;0.07499;,
 0.15000;1.17157;0.07499;,
 -0.15000;1.17157;0.07499;,
 -0.15000;1.20445;0.24720;,
 0.15000;1.20445;0.24720;,
 0.15000;1.09445;0.60532;,
 -0.15000;1.09445;0.60532;,
 -0.15000;1.29810;0.39319;,
 0.15000;1.29810;0.39319;,
 0.15000;1.32804;0.76790;,
 -0.15000;1.32804;0.76790;,
 -0.15000;1.43825;0.49074;,
 0.15000;1.43825;0.49074;,
 0.15000;1.60357;0.82499;,
 -0.15000;1.60357;0.82499;,
 -0.15000;1.60357;0.52499;,
 0.15000;1.60357;0.52499;,
 0.15000;1.87910;0.76790;,
 -0.15000;1.87910;0.76790;,
 -0.15000;1.76889;0.49074;,
 0.15000;1.76889;0.49074;,
 0.15000;2.11269;0.60532;,
 -0.15000;2.11269;0.60532;,
 -0.15000;1.90904;0.39319;,
 0.15000;1.90904;0.39319;,
 0.15000;2.26876;0.36200;,
 -0.15000;2.26876;0.36200;,
 -0.15000;2.00269;0.24720;,
 0.15000;2.00269;0.24720;,
 0.15000;2.32357;0.07499;,
 -0.15000;2.32357;0.07499;,
 -0.15000;2.03557;0.07499;,
 0.15000;2.03557;0.07499;,
 0.15000;1.17157;0.07499;,
 0.15000;1.20445;0.24720;,
 0.15000;0.93838;0.36200;,
 0.15000;0.88357;0.07499;,
 -0.15000;0.93838;0.36200;,
 -0.15000;1.20445;0.24720;,
 -0.15000;1.17157;0.07499;,
 -0.15000;0.88357;0.07499;,
 0.15000;1.29810;0.39319;,
 0.15000;1.09445;0.60532;,
 -0.15000;1.09445;0.60532;,
 -0.15000;1.29810;0.39319;,
 0.15000;1.43825;0.49074;,
 0.15000;1.32804;0.76790;,
 -0.15000;1.32804;0.76790;,
 -0.15000;1.43825;0.49074;,
 0.15000;1.60357;0.52499;,
 0.15000;1.60357;0.82499;,
 -0.15000;1.60357;0.82499;,
 -0.15000;1.60357;0.52499;,
 0.15000;1.76889;0.49074;,
 0.15000;1.87910;0.76790;,
 -0.15000;1.87910;0.76790;,
 -0.15000;1.76889;0.49074;,
 0.15000;1.90904;0.39319;,
 0.15000;2.11269;0.60532;,
 -0.15000;2.11269;0.60532;,
 -0.15000;1.90904;0.39319;,
 0.15000;2.00269;0.24720;,
 0.15000;2.26876;0.36200;,
 -0.15000;2.26876;0.36200;,
 -0.15000;2.00269;0.24720;,
 0.15000;2.03557;0.07499;,
 0.15000;2.32357;0.07499;,
 -0.15000;2.32357;0.07499;,
 -0.15000;2.03557;0.07499;,
 0.15000;2.32177;-0.22506;,
 0.15000;0.01777;-0.22506;,
 -0.15000;0.01777;-0.22506;,
 -0.15000;2.32177;-0.22506;,
 0.15000;2.32177;0.07494;,
 0.15000;0.01777;0.07494;,
 0.15000;0.01777;-0.22506;,
 0.15000;2.32177;-0.22506;,
 -0.15000;2.32177;0.07494;,
 -0.15000;0.01777;0.07494;,
 0.15000;0.01777;0.07494;,
 0.15000;2.32177;0.07494;,
 -0.15000;2.32177;-0.22506;,
 -0.15000;0.01777;-0.22506;,
 -0.15000;0.01777;0.07494;,
 -0.15000;2.32177;0.07494;,
 0.15000;2.32177;-0.22506;,
 -0.15000;2.32177;-0.22506;,
 0.15000;0.01777;-0.22506;,
 -0.15000;0.01777;-0.22506;;
 
 76;
 3;0,1,2;,
 3;0,2,3;,
 3;4,5,6;,
 3;4,6,7;,
 3;8,9,1;,
 3;8,1,0;,
 3;7,6,10;,
 3;7,10,11;,
 3;12,13,9;,
 3;12,9,8;,
 3;11,10,14;,
 3;11,14,15;,
 3;16,17,13;,
 3;16,13,12;,
 3;15,14,18;,
 3;15,18,19;,
 3;20,21,17;,
 3;20,17,16;,
 3;19,18,22;,
 3;19,22,23;,
 3;24,25,21;,
 3;24,21,20;,
 3;23,22,26;,
 3;23,26,27;,
 3;28,29,25;,
 3;28,25,24;,
 3;27,26,30;,
 3;27,30,31;,
 3;32,33,29;,
 3;32,29,28;,
 3;31,30,34;,
 3;31,34,35;,
 3;36,37,38;,
 3;36,38,39;,
 3;40,41,42;,
 3;40,42,43;,
 3;37,44,45;,
 3;37,45,38;,
 3;46,47,41;,
 3;46,41,40;,
 3;44,48,49;,
 3;44,49,45;,
 3;50,51,47;,
 3;50,47,46;,
 3;48,52,53;,
 3;48,53,49;,
 3;54,55,51;,
 3;54,51,50;,
 3;52,56,57;,
 3;52,57,53;,
 3;58,59,55;,
 3;58,55,54;,
 3;56,60,61;,
 3;56,61,57;,
 3;62,63,59;,
 3;62,59,58;,
 3;60,64,65;,
 3;60,65,61;,
 3;66,67,63;,
 3;66,63,62;,
 3;64,68,69;,
 3;64,69,65;,
 3;70,71,67;,
 3;70,67,66;,
 3;72,73,74;,
 3;72,74,75;,
 3;76,77,78;,
 3;76,78,79;,
 3;80,81,82;,
 3;80,82,83;,
 3;84,85,86;,
 3;84,86,87;,
 3;76,88,89;,
 3;76,89,87;,
 3;90,77,86;,
 3;90,86,91;;
 
 MeshMaterialList {
  1;
  76;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.000000;0.065882;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  64;
  -0.000000;-0.982252;0.187567;,
  0.000000;-0.950732;0.310015;,
  0.000000;-0.764592;0.644515;,
  0.000000;-0.456710;0.889616;,
  0.000000;-0.068903;0.997623;,
  0.000000;0.331663;0.943398;,
  0.000000;0.672880;0.739752;,
  0.000000;0.903119;0.429390;,
  0.000000;0.982252;0.187567;,
  0.000000;-0.903119;0.429391;,
  0.000000;-0.672880;0.739752;,
  0.000000;-0.331663;0.943398;,
  0.000000;0.068902;0.997623;,
  0.000000;0.456709;0.889616;,
  0.000000;0.764592;0.644514;,
  0.000000;0.950732;0.310015;,
  0.000000;0.982252;-0.187566;,
  0.000000;0.903119;-0.429391;,
  0.000000;0.672880;-0.739752;,
  0.000000;0.331663;-0.943398;,
  0.000000;-0.068902;-0.997623;,
  0.000000;-0.456710;-0.889616;,
  0.000000;-0.764592;-0.644514;,
  0.000000;-0.950732;-0.310015;,
  0.000000;-0.982252;-0.187567;,
  0.000000;0.982252;-0.187566;,
  0.000000;0.950732;-0.310015;,
  0.000000;0.764592;-0.644515;,
  0.000000;0.456710;-0.889616;,
  0.000000;0.068903;-0.997623;,
  0.000000;-0.331663;-0.943398;,
  0.000000;-0.672881;-0.739751;,
  0.000000;-0.903119;-0.429390;,
  0.000000;0.000000;-1.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;0.000000;1.000000;,
  -1.000000;0.000000;-0.000000;,
  -1.000000;0.000000;-0.000000;,
  -1.000000;0.000000;-0.000000;,
  -1.000000;0.000000;-0.000000;,
  1.000000;-0.000000;0.000000;,
  -1.000000;0.000000;-0.000000;,
  -1.000000;0.000000;-0.000000;,
  1.000000;-0.000000;0.000000;,
  -1.000000;0.000000;-0.000000;,
  -1.000000;0.000000;0.000000;,
  1.000000;-0.000000;0.000000;,
  -1.000000;0.000000;0.000000;,
  -1.000000;0.000000;0.000000;,
  1.000000;-0.000000;-0.000000;,
  1.000000;-0.000000;0.000000;,
  -1.000000;0.000000;0.000000;,
  -1.000000;0.000000;0.000000;,
  1.000000;-0.000000;-0.000000;,
  1.000000;-0.000000;-0.000000;,
  -1.000000;0.000000;-0.000000;,
  -1.000000;0.000000;-0.000000;,
  1.000000;-0.000000;-0.000000;,
  1.000000;-0.000000;-0.000000;,
  1.000000;-0.000000;-0.000000;,
  1.000000;-0.000000;-0.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;;
  76;
  3;1,9,0;,
  3;1,0,0;,
  3;16,25,26;,
  3;16,26,17;,
  3;2,10,9;,
  3;2,9,1;,
  3;17,26,27;,
  3;17,27,18;,
  3;3,11,10;,
  3;3,10,2;,
  3;18,27,28;,
  3;18,28,19;,
  3;4,12,11;,
  3;4,11,3;,
  3;19,28,29;,
  3;19,29,20;,
  3;5,13,12;,
  3;5,12,4;,
  3;20,29,30;,
  3;20,30,21;,
  3;6,14,13;,
  3;6,13,5;,
  3;21,30,31;,
  3;21,31,22;,
  3;7,15,14;,
  3;7,14,6;,
  3;22,31,32;,
  3;22,32,23;,
  3;8,8,15;,
  3;8,15,7;,
  3;23,32,24;,
  3;23,24,24;,
  3;34,34,34;,
  3;34,34,34;,
  3;36,37,38;,
  3;36,38,39;,
  3;34,40,34;,
  3;34,34,34;,
  3;41,42,37;,
  3;41,37,36;,
  3;40,43,43;,
  3;40,43,34;,
  3;44,45,42;,
  3;44,42,41;,
  3;43,34,46;,
  3;43,46,43;,
  3;47,48,45;,
  3;47,45,44;,
  3;34,49,50;,
  3;34,50,46;,
  3;51,52,48;,
  3;51,48,47;,
  3;49,53,54;,
  3;49,54,50;,
  3;55,56,52;,
  3;55,52,51;,
  3;53,57,58;,
  3;53,58,54;,
  3;45,45,56;,
  3;45,56,55;,
  3;57,59,60;,
  3;57,60,58;,
  3;45,61,45;,
  3;45,45,45;,
  3;33,33,33;,
  3;33,33,33;,
  3;34,34,34;,
  3;34,34,34;,
  3;35,35,35;,
  3;35,35,35;,
  3;45,45,45;,
  3;45,45,45;,
  3;62,62,62;,
  3;62,62,62;,
  3;63,63,63;,
  3;63,63,63;;
 }
 MeshTextureCoords {
  92;
  0.312500;0.000000;,
  0.312500;1.000000;,
  0.250000;1.000000;,
  0.250000;0.000000;,
  0.250000;0.000000;,
  0.250000;1.000000;,
  0.312500;1.000000;,
  0.312500;0.000000;,
  0.375000;0.000000;,
  0.375000;1.000000;,
  0.375000;1.000000;,
  0.375000;0.000000;,
  0.437500;0.000000;,
  0.437500;1.000000;,
  0.437500;1.000000;,
  0.437500;0.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.500000;1.000000;,
  0.500000;0.000000;,
  0.562500;0.000000;,
  0.562500;1.000000;,
  0.562500;1.000000;,
  0.562500;0.000000;,
  0.625000;0.000000;,
  0.625000;1.000000;,
  0.625000;1.000000;,
  0.625000;0.000000;,
  0.687500;0.000000;,
  0.687500;1.000000;,
  0.687500;1.000000;,
  0.687500;0.000000;,
  0.750000;0.000000;,
  0.750000;1.000000;,
  0.750000;1.000000;,
  0.750000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;;
 }
}
