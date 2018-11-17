local tiled = require("Scripting\\tiled-data")

local walls_ = {}
local routes_ = {}
local enemies_ = {}
local players_ = {}

for l=1, #tiled.layers do  
  local layer = tiled.layers[l]
  
  if layer.type == "objectgroup" then  
    for o=1, #layer.objects do      
      local object = layer.objects[o]      
      if object.shape == "polyline" then        
        if layer.name == "Walls" then          
          local prevX = nil
          local prevY = nil          
          for j=1, #object.polyline do          
            local nextX = object.x + object.polyline[j].x
            local nextY = object.y + object.polyline[j].y            
            if prevX ~= nil and prevY ~= nil then
              table.insert(walls_, {prevX, prevY, nextX, nextY})
            end            
            prevX = nextX
            prevY = nextY
          end
          
        elseif layer.name == "Routes" then          
          local rpoints = {}          
          for j=1, #object.polyline do            
            local nextX = object.x + object.polyline[j].x
            local nextY = object.y + object.polyline[j].y            
            table.insert(rpoints, {nextX, nextY})
          end          
          table.insert(routes_, {id = object.id, points = rpoints})          
        end
        
      elseif object.shape == "point" then        
        if layer.name == "Players" then
          table.insert(players_, {id = object.name, point = {object.x, object.y}})
        elseif layer.name == "Enemies" then
          table.insert(enemies_, {point = {object.x, object.y}, routeId = object.properties.Route})
        end      
      end      
    end
    
  elseif layer.type == "tilelayer" then     
    tiles_ = {numTiles = #layer.data, data = layer.data}
  end  
end

return {walls = walls_, routes = routes_, players = players_, enemies = enemies_, tiles = tiles_}