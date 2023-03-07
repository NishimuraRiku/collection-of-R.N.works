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
 132;
 4.00000;7.00000;1.00000;,
 4.00000;2.00000;-1.00000;,
 4.00000;2.00000;1.00000;,
 4.00000;7.00000;-1.00000;,
 1.00000;1.00000;0.00000;,
 1.00000;0.00000;-1.00000;,
 1.00000;0.00000;0.00000;,
 1.00000;1.00000;-1.00000;,
 1.00000;2.00000;0.00000;,
 1.00000;1.00000;-1.00000;,
 1.00000;1.00000;0.00000;,
 1.00000;2.00000;-1.00000;,
 -1.00000;0.00000;0.00000;,
 -1.00000;0.00000;-1.00000;,
 -1.00000;1.00000;0.00000;,
 -1.00000;1.00000;-1.00000;,
 -1.00000;1.00000;0.00000;,
 -1.00000;1.00000;-1.00000;,
 -1.00000;2.00000;0.00000;,
 -1.00000;2.00000;-1.00000;,
 -4.00000;2.00000;1.00000;,
 -4.00000;2.00000;-1.00000;,
 -4.00000;7.00000;1.00000;,
 -4.00000;7.00000;-1.00000;,
 3.00000;3.00000;1.00000;,
 3.00000;4.00000;1.00000;,
 3.00000;5.00000;1.00000;,
 3.00000;6.00000;1.00000;,
 0.00000;3.00000;1.00000;,
 0.00000;3.00000;1.00000;,
 3.00000;4.00000;1.00000;,
 3.00000;3.00000;1.00000;,
 0.00000;4.00000;1.00000;,
 0.00000;4.00000;1.00000;,
 -1.00000;3.00000;1.00000;,
 -1.00000;4.00000;1.00000;,
 -2.00000;3.00000;1.00000;,
 -2.00000;3.00000;1.00000;,
 -1.00000;4.00000;1.00000;,
 -1.00000;3.00000;1.00000;,
 -2.00000;4.00000;1.00000;,
 -2.00000;4.00000;1.00000;,
 -3.00000;5.00000;1.00000;,
 3.00000;6.00000;1.00000;,
 3.00000;5.00000;1.00000;,
 -3.00000;5.00000;1.00000;,
 -3.00000;6.00000;1.00000;,
 -3.00000;6.00000;1.00000;,
 3.00000;2.00000;-1.00000;,
 3.00000;6.00000;-1.00000;,
 3.00000;2.00000;-1.00000;,
 3.00000;6.00000;-1.00000;,
 1.00000;2.00000;-1.00000;,
 -1.00000;2.00000;-1.00000;,
 -3.00000;2.00000;-1.00000;,
 -3.00000;6.00000;-1.00000;,
 -3.00000;6.00000;-1.00000;,
 -3.00000;2.00000;-1.00000;,
 3.00000;2.00000;0.00000;,
 1.00000;2.00000;0.00000;,
 -1.00000;2.00000;0.00000;,
 -3.00000;2.00000;0.00000;,
 1.00000;2.00000;0.00000;,
 3.00000;2.00000;0.00000;,
 -3.00000;2.00000;0.00000;,
 -1.00000;2.00000;0.00000;,
 4.20000;7.17500;1.05000;,
 4.20000;1.92500;1.05000;,
 4.20000;1.92500;-1.05000;,
 4.20000;7.17500;-1.05000;,
 1.05000;0.87500;0.00000;,
 1.05000;-0.17500;0.00000;,
 1.05000;-0.17500;-1.05000;,
 1.05000;0.87500;-1.05000;,
 1.05000;1.92500;0.00000;,
 1.05000;0.87500;0.00000;,
 1.05000;0.87500;-1.05000;,
 1.05000;1.92500;-1.05000;,
 -1.05000;-0.17500;0.00000;,
 -1.05000;0.87500;0.00000;,
 -1.05000;-0.17500;-1.05000;,
 -1.05000;0.87500;-1.05000;,
 -1.05000;0.87500;0.00000;,
 -1.05000;1.92500;0.00000;,
 -1.05000;0.87500;-1.05000;,
 -1.05000;1.92500;-1.05000;,
 -4.20000;1.92500;1.05000;,
 -4.20000;7.17500;1.05000;,
 -4.20000;1.92500;-1.05000;,
 -4.20000;7.17500;-1.05000;,
 3.15000;2.97500;1.05000;,
 3.15000;4.02500;1.05000;,
 3.15000;5.07500;1.05000;,
 3.15000;6.12500;1.05000;,
 0.00000;2.97500;1.05000;,
 0.00000;2.97500;1.05000;,
 3.15000;2.97500;1.05000;,
 3.15000;4.02500;1.05000;,
 0.00000;4.02500;1.05000;,
 0.00000;4.02500;1.05000;,
 -1.05000;2.97500;1.05000;,
 -1.05000;4.02500;1.05000;,
 -2.10000;2.97500;1.05000;,
 -2.10000;2.97500;1.05000;,
 -1.05000;2.97500;1.05000;,
 -1.05000;4.02500;1.05000;,
 -2.10000;4.02500;1.05000;,
 -2.10000;4.02500;1.05000;,
 -3.15000;5.07500;1.05000;,
 3.15000;5.07500;1.05000;,
 3.15000;6.12500;1.05000;,
 -3.15000;5.07500;1.05000;,
 -3.15000;6.12500;1.05000;,
 -3.15000;6.12500;1.05000;,
 3.15000;1.92500;-1.05000;,
 3.15000;6.12500;-1.05000;,
 3.15000;1.92500;-1.05000;,
 1.05000;1.92500;-1.05000;,
 3.15000;6.12500;-1.05000;,
 -1.05000;1.92500;-1.05000;,
 -3.15000;1.92500;-1.05000;,
 -3.15000;6.12500;-1.05000;,
 -3.15000;6.12500;-1.05000;,
 -3.15000;1.92500;-1.05000;,
 3.15000;1.92500;0.00000;,
 1.05000;1.92500;0.00000;,
 -1.05000;1.92500;0.00000;,
 -3.15000;1.92500;0.00000;,
 3.15000;1.92500;0.00000;,
 1.05000;1.92500;0.00000;,
 -1.05000;1.92500;0.00000;,
 -3.15000;1.92500;0.00000;;
 
 144;
 3;2,1,0;,
 3;0,1,3;,
 3;6,5,4;,
 3;4,5,7;,
 3;10,9,8;,
 3;8,9,11;,
 3;14,13,12;,
 3;15,13,14;,
 3;18,17,16;,
 3;19,17,18;,
 3;22,21,20;,
 3;23,21,22;,
 3;2,0,24;,
 3;24,0,25;,
 3;25,0,26;,
 3;26,0,27;,
 3;2,24,28;,
 3;31,30,29;,
 3;25,26,32;,
 3;29,30,33;,
 3;2,28,34;,
 3;28,32,34;,
 3;32,26,35;,
 3;34,32,35;,
 3;2,34,36;,
 3;39,38,37;,
 3;35,26,40;,
 3;37,38,41;,
 3;44,43,42;,
 3;40,26,45;,
 3;36,40,45;,
 3;27,0,46;,
 3;42,43,47;,
 3;36,45,20;,
 3;2,36,20;,
 3;45,46,20;,
 3;46,0,22;,
 3;20,46,22;,
 3;6,4,12;,
 3;10,8,16;,
 3;12,4,14;,
 3;16,8,18;,
 3;48,3,1;,
 3;49,3,48;,
 3;52,51,50;,
 3;13,7,5;,
 3;17,11,9;,
 3;15,7,13;,
 3;53,51,52;,
 3;19,11,17;,
 3;54,51,53;,
 3;55,3,49;,
 3;56,51,54;,
 3;21,55,57;,
 3;23,3,55;,
 3;23,55,21;,
 3;6,12,5;,
 3;5,12,13;,
 3;2,20,58;,
 3;58,20,59;,
 3;59,20,60;,
 3;60,20,61;,
 3;2,58,1;,
 3;63,62,50;,
 3;1,58,48;,
 3;50,62,52;,
 3;65,64,53;,
 3;61,20,57;,
 3;53,64,54;,
 3;57,20,21;,
 3;3,22,0;,
 3;23,22,3;,
 3;68,67,66;,
 3;68,66,69;,
 3;72,71,70;,
 3;72,70,73;,
 3;76,75,74;,
 3;76,74,77;,
 3;80,79,78;,
 3;80,81,79;,
 3;84,83,82;,
 3;84,85,83;,
 3;88,87,86;,
 3;88,89,87;,
 3;66,67,90;,
 3;66,90,91;,
 3;66,91,92;,
 3;66,92,93;,
 3;90,67,94;,
 3;97,96,95;,
 3;92,91,98;,
 3;97,95,99;,
 3;94,67,100;,
 3;98,94,100;,
 3;92,98,101;,
 3;98,100,101;,
 3;100,67,102;,
 3;105,104,103;,
 3;92,101,106;,
 3;105,103,107;,
 3;110,109,108;,
 3;92,106,111;,
 3;106,102,111;,
 3;66,93,112;,
 3;110,108,113;,
 3;111,102,86;,
 3;102,67,86;,
 3;112,111,86;,
 3;66,112,87;,
 3;112,86,87;,
 3;70,71,78;,
 3;74,75,82;,
 3;70,78,79;,
 3;74,82,83;,
 3;69,114,68;,
 3;69,115,114;,
 3;118,117,116;,
 3;73,80,72;,
 3;77,84,76;,
 3;73,81,80;,
 3;118,119,117;,
 3;77,85,84;,
 3;118,120,119;,
 3;69,121,115;,
 3;118,122,120;,
 3;121,88,123;,
 3;69,89,121;,
 3;121,89,88;,
 3;78,71,72;,
 3;78,72,80;,
 3;86,67,124;,
 3;86,124,125;,
 3;86,125,126;,
 3;86,126,127;,
 3;124,67,68;,
 3;129,128,116;,
 3;124,68,114;,
 3;129,116,117;,
 3;131,130,119;,
 3;86,127,123;,
 3;131,119,120;,
 3;86,123,88;,
 3;87,69,66;,
 3;87,89,69;;
 
 MeshMaterialList {
  2;
  144;
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
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\Material\\mtl_pallet_000.png";
   }
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  13;
  1.000000;0.000000;0.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;0.000000;1.000000;,
  0.000000;0.000000;-1.000000;,
  0.000000;-1.000000;-0.000000;,
  -0.000000;-1.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  -1.000000;0.000000;0.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;0.000000;-1.000000;,
  0.000000;0.000000;1.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;0.000000;;
  144;
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;2,2,2;,
  3;2,2,2;,
  3;2,2,2;,
  3;2,2,2;,
  3;2,2,2;,
  3;2,2,2;,
  3;2,2,2;,
  3;2,2,2;,
  3;2,2,2;,
  3;2,2,2;,
  3;2,2,2;,
  3;2,2,2;,
  3;2,2,2;,
  3;2,2,2;,
  3;2,2,2;,
  3;2,2,2;,
  3;2,2,2;,
  3;2,2,2;,
  3;2,2,2;,
  3;2,2,2;,
  3;2,2,2;,
  3;2,2,2;,
  3;2,2,2;,
  3;2,2,2;,
  3;2,2,2;,
  3;2,2,2;,
  3;2,2,2;,
  3;2,2,2;,
  3;2,2,2;,
  3;2,2,2;,
  3;3,3,3;,
  3;3,3,3;,
  3;3,3,3;,
  3;3,3,3;,
  3;3,3,3;,
  3;3,3,3;,
  3;3,3,3;,
  3;3,3,3;,
  3;3,3,3;,
  3;3,3,3;,
  3;3,3,3;,
  3;3,3,3;,
  3;3,3,3;,
  3;3,3,3;,
  3;4,4,4;,
  3;4,4,4;,
  3;4,4,4;,
  3;4,4,4;,
  3;4,4,4;,
  3;4,4,4;,
  3;4,4,4;,
  3;4,4,4;,
  3;4,4,4;,
  3;4,4,5;,
  3;4,4,4;,
  3;4,4,4;,
  3;4,4,4;,
  3;4,4,4;,
  3;6,6,6;,
  3;6,6,6;,
  3;7,7,7;,
  3;7,7,7;,
  3;7,7,7;,
  3;7,7,7;,
  3;7,7,7;,
  3;7,7,7;,
  3;8,8,8;,
  3;8,8,8;,
  3;8,8,8;,
  3;8,8,8;,
  3;8,8,8;,
  3;8,8,8;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;10,10,10;,
  3;10,10,10;,
  3;10,10,10;,
  3;10,10,10;,
  3;10,10,10;,
  3;10,10,10;,
  3;10,10,10;,
  3;10,10,10;,
  3;10,10,10;,
  3;10,10,10;,
  3;10,10,10;,
  3;10,10,10;,
  3;10,10,10;,
  3;10,10,10;,
  3;11,11,11;,
  3;11,11,11;,
  3;11,11,11;,
  3;11,11,11;,
  3;11,11,11;,
  3;11,11,11;,
  3;11,11,11;,
  3;11,11,11;,
  3;11,11,11;,
  3;11,11,11;,
  3;11,11,11;,
  3;11,11,11;,
  3;11,11,11;,
  3;11,11,11;,
  3;12,12,12;,
  3;12,12,12;;
 }
 MeshTextureCoords {
  132;
  0.712891;0.500000;,
  0.712891;0.500000;,
  0.712891;0.500000;,
  0.712891;0.500000;,
  0.708984;0.500000;,
  0.708984;0.500000;,
  0.708984;0.500000;,
  0.708984;0.500000;,
  0.705078;0.500000;,
  0.705078;0.500000;,
  0.705078;0.500000;,
  0.705078;0.500000;,
  0.708984;0.500000;,
  0.708984;0.500000;,
  0.708984;0.500000;,
  0.708984;0.500000;,
  0.705078;0.500000;,
  0.705078;0.500000;,
  0.705078;0.500000;,
  0.705078;0.500000;,
  0.712891;0.500000;,
  0.712891;0.500000;,
  0.712891;0.500000;,
  0.712891;0.500000;,
  0.712891;0.500000;,
  0.712891;0.500000;,
  0.712891;0.500000;,
  0.712891;0.500000;,
  0.712891;0.500000;,
  0.708984;0.500000;,
  0.708984;0.500000;,
  0.708984;0.500000;,
  0.712891;0.500000;,
  0.708984;0.500000;,
  0.712891;0.500000;,
  0.712891;0.500000;,
  0.712891;0.500000;,
  0.708984;0.500000;,
  0.708984;0.500000;,
  0.708984;0.500000;,
  0.712891;0.500000;,
  0.708984;0.500000;,
  0.708984;0.500000;,
  0.708984;0.500000;,
  0.708984;0.500000;,
  0.712891;0.500000;,
  0.712891;0.500000;,
  0.708984;0.500000;,
  0.712891;0.500000;,
  0.712891;0.500000;,
  0.708984;0.500000;,
  0.708984;0.500000;,
  0.708984;0.500000;,
  0.708984;0.500000;,
  0.708984;0.500000;,
  0.712891;0.500000;,
  0.708984;0.500000;,
  0.712891;0.500000;,
  0.712891;0.500000;,
  0.712891;0.500000;,
  0.712891;0.500000;,
  0.712891;0.500000;,
  0.708984;0.500000;,
  0.708984;0.500000;,
  0.708984;0.500000;,
  0.708984;0.500000;,
  0.712891;0.500000;,
  0.712891;0.500000;,
  0.712891;0.500000;,
  0.712891;0.500000;,
  0.708984;0.500000;,
  0.708984;0.500000;,
  0.708984;0.500000;,
  0.708984;0.500000;,
  0.705078;0.500000;,
  0.705078;0.500000;,
  0.705078;0.500000;,
  0.705078;0.500000;,
  0.708984;0.500000;,
  0.708984;0.500000;,
  0.708984;0.500000;,
  0.708984;0.500000;,
  0.705078;0.500000;,
  0.705078;0.500000;,
  0.705078;0.500000;,
  0.705078;0.500000;,
  0.712891;0.500000;,
  0.712891;0.500000;,
  0.712891;0.500000;,
  0.712891;0.500000;,
  0.712891;0.500000;,
  0.712891;0.500000;,
  0.712891;0.500000;,
  0.712891;0.500000;,
  0.712891;0.500000;,
  0.708984;0.500000;,
  0.708984;0.500000;,
  0.708984;0.500000;,
  0.712891;0.500000;,
  0.708984;0.500000;,
  0.712891;0.500000;,
  0.712891;0.500000;,
  0.712891;0.500000;,
  0.708984;0.500000;,
  0.708984;0.500000;,
  0.708984;0.500000;,
  0.712891;0.500000;,
  0.708984;0.500000;,
  0.708984;0.500000;,
  0.708984;0.500000;,
  0.708984;0.500000;,
  0.712891;0.500000;,
  0.712891;0.500000;,
  0.708984;0.500000;,
  0.712891;0.500000;,
  0.712891;0.500000;,
  0.708984;0.500000;,
  0.708984;0.500000;,
  0.708984;0.500000;,
  0.708984;0.500000;,
  0.708984;0.500000;,
  0.712891;0.500000;,
  0.708984;0.500000;,
  0.712891;0.500000;,
  0.712891;0.500000;,
  0.712891;0.500000;,
  0.712891;0.500000;,
  0.712891;0.500000;,
  0.708984;0.500000;,
  0.708984;0.500000;,
  0.708984;0.500000;,
  0.708984;0.500000;;
 }
}
